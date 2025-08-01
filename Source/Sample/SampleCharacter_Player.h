// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SampleCharacter_Base.h"
#include "InputAction.h"
#include "SampleCharacter_Player.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ASampleCharacter_Player : public ASampleCharacter_Base
{
	GENERATED_BODY()
public:
	ASampleCharacter_Player();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input

	virtual void MoveCharacter(const FInputActionInstance& Instance);

	virtual void RotateCharacter(const FInputActionInstance& Instance);

	void ToggleLockCharacterTurn(const FInputActionInstance& Instance);

	void StartCrouch(const FInputActionInstance& Instance);

	void StartFire(const FInputActionInstance& Instance);

	void StopFire(const FInputActionInstance& Instance);

public:

	// TurnInPlace

	bool IsLockCharacterTurn() { return bLockCharacterTurn; }

	// Weapon

	void AddWeapon(const FString WeapClassName);
	void ChangeWeapon(const int WeaponIdx);
	UFUNCTION(BlueprintPure)
	bool GetIsFiring() { return bIsFiring; }
	
private:

	// Camera

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TurnRate = 90.f;

	float TurnLimit_Yaw = 90.f;

	bool bLockCharacterTurn = false;

	// Input

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> RotateInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> LockCharacterTurnInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> CrouchInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> FireInputAction;

	// Movement

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 300.f;

	// Weapon

	bool bIsFiring = false;
	int CurrentWeaponIndex = 0;

	TObjectPtr<class ASPWeapon> CurrentWeapon;
	TArray < TObjectPtr<class ASPWeapon> > Weapons;

	// Widget

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Widget")
	TSubclassOf<class UUserWidget> WidgetClass;
	UUserWidget* PlayerWidget;

};