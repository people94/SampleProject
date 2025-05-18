// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleAnimInstance_Character.h"
#include "SampleCharacter_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

USampleAnimInstance_Character::USampleAnimInstance_Character()
{
}

void USampleAnimInstance_Character::NativeInitializeAnimation()
{
	OwnerPawn = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerPawn)
	{
		OwnerMovementComponent = OwnerPawn->GetCharacterMovement();
	}
}

void USampleAnimInstance_Character::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerPawn)
	{
		if (OwnerMovementComponent)
		{
			IsJump = OwnerMovementComponent->IsFalling();
		}
		if(!IsJump)
		{
			Speed = OwnerPawn->GetVelocity().Size();
			Direction = CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
		}
		FRotator DeltaRotation = OwnerPawn->GetBaseAimRotation() - OwnerPawn->GetActorRotation();
		Pitch = DeltaRotation.Pitch;
		Yaw = DeltaRotation.Yaw;
		
		IsCrouch = OwnerPawn->bIsCrouched;

		UE_LOG(LogTemp, Warning, TEXT("Speed = %f Direction = %f DeltaRotation = %s IsJump = %d IsCrouch = %d"), Speed, Direction, *DeltaRotation.ToString(), IsJump, IsCrouch);
	}
}
