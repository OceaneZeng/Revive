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
	USphereComponent* PickUpSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraEffect;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Weapon)
	void BeginShoot();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Weapon)
	void EndShoot();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Weapon)
	void Reload();

	UFUNCTION(BlueprintCallable, Category=Ammo)
	int32 ComputeLeftAmmo();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Ammo)
	int32 CurAmmo = 30;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Ammo)
	int32 MaxAmmoPerClip = 30;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Ammo)
	int32 TotalAmmo = 200;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Ammo)
	int32 LeftAmmo = 0;


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
