// Fill out your copyright notice in the Description page of Project Settings.


#include "ReviveWeapon.h"


// Sets default values
AReviveWeapon::AReviveWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建武器网格体
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh); // 设置武器为根组件
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

