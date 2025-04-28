// Fill out your copyright notice in the Description page of Project Settings.


#include "ReviveWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AReviveWeapon::AReviveWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建骨骼网格体组件
	PickUpSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(PickUpSphere); // 把它设为RootComponent
	PickUpSphere->SetSphereRadius(150.f); // 设置范围半径，比如 150cm

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(PickUpSphere);
	
	// 创建Niagara粒子组件
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(SkeletalMesh); // 粒子附着在骨骼网格体上
	
	NiagaraEffect->bAutoActivate = true;
}

// Called when the game starts or when spawned
void AReviveWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReviveWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AReviveWeapon::OnPickedUp_Implementation()
{
	if (NiagaraEffect)
	{
		NiagaraEffect->Deactivate(); // 关闭特效
	}
}

void AReviveWeapon::OnDropped_Implementation()
{
	if (NiagaraEffect)
	{
		NiagaraEffect->Activate(); // 开启特效
	}
}
