// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReviveWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class REVIVE_API AReviveWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PickUpSphereComponent;

	FTimerHandle PickupTimerHandle; // 定时器句柄

public:
	// Sets default values for this actor's properties
	AReviveWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 被拾取时调用
	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();

	// 被丢弃时调用
	UFUNCTION(BlueprintNativeEvent)
	void OnDropped();
};
