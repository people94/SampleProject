// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPCharacter_Base.h"
#include "InputAction.h"
#include "SPCharacter_Player.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemBeginTouched, FString);
DECLARE_MULTICAST_DELEGATE(FOnItemEndTouched);

UCLASS()
class SAMPLE_API ASPCharacter_Player : public ASPCharacter_Base
{
	GENERATED_BODY()
public:
	ASPCharacter_Player();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input

public:
	virtual void MoveCharacter(const FVector InputVector);

	virtual void RotateCharacter(const FVector InputVector);

	void ToggleLockCharacterTurn(const bool InputValue);

	void StartCrouch(const bool InputValue);

	void StartFire();

	void StopFire();

	void HandleInteraction();

	void HandleWeaponSelect(int SelectIndex);
	void HandleUnEquipWeapon();
public:

	// Camera

	TObjectPtr<UCameraComponent> GetCameraComponent() { return Camera; }

	// TurnInPlace

	bool IsLockCharacterTurn() { return bLockCharacterTurn; }

	// Item

	virtual void SetTouchItem(ASPItem* Item) override;

	virtual ASPItem* GetTouchItem() { return TouchItem; }
	
protected:

	// Camera

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TurnRate = 90.f;

	float TurnLimit_Yaw = 90.f;

	bool bLockCharacterTurn = false;

public:
	FOnItemBeginTouched OnItemBeginTouched;
	FOnItemEndTouched OnItemEndTouched;
};