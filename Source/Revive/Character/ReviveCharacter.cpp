// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReviveCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Revive/Weapon/ReviveWeapon.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AReviveCharacter

AReviveCharacter::AReviveCharacter()
{
	// 设置胶囊体碰撞大小
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character 朝向移动方向	

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // 俯视角度
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 3.0f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	static ConstructorHelpers::FClassFinder<AActor> WeaponBPClass(TEXT("/Game/Blueprints/Weapon/BP_Weapon"));
	if (WeaponBPClass.Class != nullptr)
	{
		DefaultWeaponClass = WeaponBPClass.Class;
	}
}

void AReviveCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// 生成武器
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 在角色的手上生成武器
		CurWeapon = GetWorld()->SpawnActor<AReviveWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
		// 如果武器生成成功，则附加到角色骨骼
		if (CurWeapon)
		{
			// 附加到骨骼（假设骨骼插槽名为 "WeaponSocket"）
			CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AReviveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AReviveCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AReviveCharacter::Look);

		//Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AReviveCharacter::BeginFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AReviveCharacter::EndFire);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AReviveCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 获取摄像机的朝向，确保角色始终按照摄像机方向移动
		FRotator CameraRotation = FollowCamera->GetComponentRotation();
		CameraRotation.Pitch = 0.f; // 确保 Pitch 不影响方向
		CameraRotation.Roll = 0.f;

		FVector ForwardDirection = CameraRotation.Vector();
		FVector RightDirection = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AReviveCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		FVector MouseWorldLocation = HitResult.ImpactPoint;
		FVector CharacterLocation = GetActorLocation();

		FRotator TargetRotation = (MouseWorldLocation - CharacterLocation).Rotation();
		TargetRotation.Pitch = 0.f; // 只让角色旋转 Yaw，不影响 Pitch
		TargetRotation.Roll = 0.f;

		// 使用插值，使旋转更平滑
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
		SetActorRotation(NewRotation);
	}
}