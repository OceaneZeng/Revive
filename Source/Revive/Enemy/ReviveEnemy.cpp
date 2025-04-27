// Fill out your copyright notice in the Description page of Project Settings.


#include "ReviveEnemy.h"


// Sets default values
AReviveEnemy::AReviveEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReviveEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReviveEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AReviveEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

