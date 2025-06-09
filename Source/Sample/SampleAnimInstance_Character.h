// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SampleAnimInstance_Character.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API USampleAnimInstance_Character : public UAnimInstance
{
	GENERATED_BODY()

public:
	USampleAnimInstance_Character();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ASampleCharacter_Player* OwnerPawn;
	class UCharacterMovementComponent* OwnerMovementComponent;
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Direction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Yaw = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Pitch = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsJump = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsCrouch = 0.0f;

	// Turn In Place Properties

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RootYawOffset = 0.0f;
	float LastRootYaw = 0.0f;
	float CharacterYaw = 0.0f;
	float LastCurveValue = 0.0f;
	float DistanceCurve = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TurnLimit_Yaw = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TurnInPlaceSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName BaseBoneName = "pelvis";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName CompareBoneName = "spine_02";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TurnInPlaceYaw = 0.0f;

	bool bTurning = false;
	float StartYaw = 0.0f;
	float TargetYaw = 0.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//bool IsSprint = 0.0f;
public:
	void SetTurnInPlaceYaw(float value) { TurnInPlaceYaw = value; }

protected:
	// Turn In Place Functions

	void TurnInPlace();
	void TurnInPlace1();
	void TurnInPlace2();
};
