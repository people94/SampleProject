// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPItem.h"
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
class SAMPLE_API ASPWeapon : public ASPItem
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
	float GetDamage() { return Damage; }
	TSubclassOf<UDamageType> GetDamageType() { return DamageType; }

protected:
	// Firing

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Firing")
	float fFireInterval = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Firing")
	float fFireRange = 0.0f;

	bool bIsFiring = false;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Firing")
	EFireType FireType = EFireType::EF_None;

	FTimerHandle FireTimerHandle;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Firing")
	float Damage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;
};
