// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "SPWeapon.generated.h"

UENUM()
enum class EFireType : uint8
{
	EF_None			UMETA(DisplayName = "None"),
	EF_LineTrace	UMETA(DisplayName = "Line Trace"),
	EF_Projectile	UMETA(DisplayName = "Projectile"),
};

UCLASS(Abstract)
class SAMPLE_API ASPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPWeapon();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Firing

	virtual void StartFire();
	virtual void StopFire();
	virtual void HandleFire();
	virtual void FireProjectile() {}
	virtual void FireLineTrace() {}
	void SetIsFiring(bool IsFiring) { bIsFiring = IsFiring; }
	bool GetIsFiring() { return bIsFiring; }

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess ="true"), Category="Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Projectile")
	TObjectPtr<class USceneComponent> ProjectileSpawnPosition;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Projectile")
	TSubclassOf<class ASPProjectile> ProjectileClass;

	TObjectPtr<class ASPProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Firing")
	float fFireInterval = 5.0f;
	bool bIsFiring = false;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Firing")
	EFireType FireType = EFireType::EF_None;

	FTimerHandle FireTimerHandle;
};
