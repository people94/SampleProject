// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleAnimInstance_Character.h"
#include "SampleCharacter_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

USampleAnimInstance_Character::USampleAnimInstance_Character()
{
}

void USampleAnimInstance_Character::NativeInitializeAnimation()
{
	OwnerPawn = Cast<ASampleCharacter_Player>(TryGetPawnOwner());
	if (OwnerPawn)
	{
		OwnerMovementComponent = OwnerPawn->GetCharacterMovement();
		Mesh = OwnerPawn->GetMesh();
	}
}

void USampleAnimInstance_Character::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerPawn)
	{
		FRotator CurrentRotation = OwnerPawn->GetActorRotation();
		if (OwnerMovementComponent)
		{
			IsJump = OwnerMovementComponent->IsFalling();
		}
		if(!IsJump)
		{
			Speed = OwnerPawn->GetVelocity().Size();
			Direction = CalculateDirection(OwnerPawn->GetVelocity(), CurrentRotation);
		}
		FRotator DeltaRotation = OwnerPawn->GetBaseAimRotation() - CurrentRotation;
		DeltaRotation = DeltaRotation.GetNormalized();
		Pitch = DeltaRotation.Pitch;
		Yaw = DeltaRotation.Yaw;
		
		
		IsCrouch = OwnerPawn->bIsCrouched;

		// ���ڸ� ȸ��
		TurnInPlace();
		//UE_LOG(LogTemp, Warning, TEXT("RootYawOffset = %f LastYaw = %f"), RootYawOffset, LastRootYaw);
	}
}

void USampleAnimInstance_Character::TurnInPlace()
{
	if (OwnerPawn)
	{
		if (!OwnerPawn->IsLockCharacterTurn())
		{
			//TurnInPlace1();
			if (FMath::IsNearlyEqual(Speed, 0.0f))
				TurnInPlace2();
			else
			{
				RootYawOffset = 0.0f;
			}
			LastRootYaw = OwnerPawn->GetActorRotation().Yaw;
		}
	}
}

// �������Ʈ ��Ʃ�� ���Ǹ� �״�� C++�� �ű� �� (0���� ����� �ڵ� ���� ����)
void USampleAnimInstance_Character::TurnInPlace1()
{
	float YawchangeOverFrame = LastRootYaw - CharacterYaw;
	float MultiplySign;
	LastRootYaw = CharacterYaw;
	CharacterYaw = OwnerPawn->GetActorRotation().Yaw;
	RootYawOffset = FRotator::NormalizeAxis(RootYawOffset + YawchangeOverFrame);
	if (GetCurveValue(FName(TEXT("Turning"))) > 0.0f)
	{
		float DistanceurveLastFrame = DistanceCurve;
		DistanceCurve = GetCurveValue(FName(TEXT("DistanceCurve")));

		if (RootYawOffset > 0.0f)
			MultiplySign = -1.0f;
		else
			MultiplySign = 1.0f;
		float distanceCurveDifference = DistanceurveLastFrame - DistanceCurve;

		RootYawOffset = RootYawOffset - (distanceCurveDifference * MultiplySign);

		//UE_LOG(LogTemp, Warning, TEXT("RootYawOffset = %f distanceCurveDifference = %f"), RootYawOffset, distanceCurveDifference * MultiplySign);
		float AbsRootYawOffset = FMath::Abs(RootYawOffset);
		if (AbsRootYawOffset > TurnLimit_Yaw)
		{
			float YawToSubtract = AbsRootYawOffset - TurnLimit_Yaw;
			if (RootYawOffset > 0.0f)
				YawToSubtract *= 1.0f;
			else
				YawToSubtract *= -1.0f;
			RootYawOffset -= YawToSubtract;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("RootYawOffset = %f"), RootYawOffset);
}

// ���� �����Ѵ�� ���� ��
void USampleAnimInstance_Character::TurnInPlace2()
{
	RootYawOffset -= OwnerPawn->GetActorRotation().Yaw - LastRootYaw; // ���� ƽ�� ȸ���Ѹ�ŭ �ݴ�� ȸ����Ű�� ���� ����.

	// ȸ�� �ִϸ��̼� �����
	if (GetCurveValue(FName(TEXT("Turning"))) > 0.0f)
	{
		///float AbsRootYawOffset = FMath::Abs(RootYawOffset);
		//float MultiplySign;
		float CurrentCurveValue = GetCurveValue(FName(TEXT("DistanceCurve")));
		// �̷��� �ϸ� �ȵ�. �ִϸ��̼� ��ȯ�� BlendTime�� ���� CurveValue�� ������ ��ġ��� �ȳ��� �� ����. ���� 0�̻��� ���� ���� �� �����Ƿ� ���밪ó���ϸ� �ȵȴ�.
		float DeltaCurveValue = CurrentCurveValue - LastCurveValue; // ���� ƽ���� ȸ���Ѿ� �ִϸ��̼ǻ� CurveValue�� ������ �����Ѵ�. �׷��� �� ���� ���������� ū�����°Ŵϱ� ������.
		// ����ȸ���� �ϴ� ������ ȸ���� �ϴ� ������ �׷��ٰ� ���������� ����� ��ȯ�� -> ������� Blend ������ �߻��ϴ� ������ Current - Last �ϸ� ȸ������ ���´ٴ� �͸� �߿���.

		// ��ǥ�� RootYawOffset�� 0���� ����� ��
		if (RootYawOffset > 0.0f) // ���� ȸ��
		{
			RootYawOffset -= DeltaCurveValue;
		}
		else
		{
			RootYawOffset += DeltaCurveValue;
		}

		// �� �κ��� ������ �߿��ϴ�. RootYawOffset�� -TurnLimit_Yaw ~ TurnLimit_Yaw ���̷� Clamp ���ش�. ���������� ������ �� ���� Animation Blend�� �����ϱ� �����ڵ�
		// �׷��� �α����� -TurnLimit_Yaw ~ TurnLimit_Yaw �� �ʰ��ϴ� ���� �������Ѵ�.
		//if (AbsRootYawOffset > TurnLimit_Yaw)
		//{
		//	float YawToSubtract = AbsRootYawOffset - TurnLimit_Yaw;
		//	if (RootYawOffset > 0.0f)
		//		YawToSubtract *= 1.0f;//
		//	else//if (RootYawOffset > TurnLimit_Yaw) // ����ȸ��
		//		YawToSubtract *= -1.0f;//	RootYawOffset = TurnLimit_Yaw;
		//	RootYawOffset -= YawToSubtract;//else if (RootYawOffset < -TurnLimit_Yaw) // ������ȸ��
		//}//	RootYawOffset = -TurnLimit_Yaw;
		// �̷��� �ص� ���� ����� ���´�. ���߿� ������ �߻��Ϸ���?
		RootYawOffset = FMath::ClampAngle(RootYawOffset, -TurnLimit_Yaw, TurnLimit_Yaw);
		LastCurveValue = CurrentCurveValue;
	}
	else
		LastCurveValue = 0.0f;
}
