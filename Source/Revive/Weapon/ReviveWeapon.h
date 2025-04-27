// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReviveWeapon.generated.h"

UCLASS()
class REVIVE_API AReviveWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReviveWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
