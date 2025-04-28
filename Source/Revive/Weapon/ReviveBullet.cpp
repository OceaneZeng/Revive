// Fill out your copyright notice in the Description page of Project Settings.


#include "ReviveBullet.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AReviveBullet::AReviveBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetSphereRadius(5.f);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(DamageSphere);
	
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(BulletMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(DamageSphere); // 告诉移动组件，它控制谁，一般是碰撞体
	ProjectileMovement->InitialSpeed = 5000.f; // 初速度
	ProjectileMovement->MaxSpeed = 7000.f; // 最大速度
	ProjectileMovement->bRotationFollowsVelocity = true; // 朝向飞行方向自动旋转
	ProjectileMovement->bShouldBounce = false; // 不弹跳，或者改成true看情况
}

// Called when the game starts or when spawned
void AReviveBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReviveBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

