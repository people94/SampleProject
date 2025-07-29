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

		// 제자리 회전
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

// 블루프린트 유튜브 강의를 그대로 C++로 옮긴 것 (0으로 만드는 코드 등은 없다)
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

// 내가 이해한대로 만든 것
void USampleAnimInstance_Character::TurnInPlace2()
{
	RootYawOffset -= OwnerPawn->GetActorRotation().Yaw - LastRootYaw; // 현재 틱에 회전한만큼 반대로 회전시키기 위해 저장.

	// 회전 애니메이션 재생중
	if (GetCurveValue(FName(TEXT("Turning"))) > 0.0f)
	{
		///float AbsRootYawOffset = FMath::Abs(RootYawOffset);
		//float MultiplySign;
		float CurrentCurveValue = GetCurveValue(FName(TEXT("DistanceCurve")));
		// 이렇게 하면 안됨. 애니메이션 전환시 BlendTime에 따라 CurveValue가 예상한 수치대로 안나올 수 있음. 따라서 0이상의 값이 나올 수 있으므로 절대값처리하면 안된다.
		float DeltaCurveValue = CurrentCurveValue - LastCurveValue; // 현재 틱에서 회전한양 애니메이션상 CurveValue가 음수로 시작한다. 그러면 이 값은 작은값에서 큰값빼는거니까 음수다.
		// 왼쪽회전을 하던 오른쪽 회전을 하던 음수임 그러다가 언젠가부터 양수로 변환됨 -> 상관없음 Blend 때문에 발생하는 문제고 Current - Last 하면 회전양이 나온다는 것만 중요함.

		// 목표는 RootYawOffset을 0으로 만드는 것
		if (RootYawOffset > 0.0f) // 왼쪽 회전
		{
			RootYawOffset -= DeltaCurveValue;
		}
		else
		{
			RootYawOffset += DeltaCurveValue;
		}

		// 이 부분이 굉장히 중요하다. RootYawOffset을 -TurnLimit_Yaw ~ TurnLimit_Yaw 사이로 Clamp 해준다. 마찬가지로 예측할 수 없는 Animation Blend를 방지하기 위한코드
		// 그러나 로그찍어보면 -TurnLimit_Yaw ~ TurnLimit_Yaw 를 초과하는 값이 나오긴한다.
		//if (AbsRootYawOffset > TurnLimit_Yaw)
		//{
		//	float YawToSubtract = AbsRootYawOffset - TurnLimit_Yaw;
		//	if (RootYawOffset > 0.0f)
		//		YawToSubtract *= 1.0f;//
		//	else//if (RootYawOffset > TurnLimit_Yaw) // 왼쪽회전
		//		YawToSubtract *= -1.0f;//	RootYawOffset = TurnLimit_Yaw;
		//	RootYawOffset -= YawToSubtract;//else if (RootYawOffset < -TurnLimit_Yaw) // 오른쪽회전
		//}//	RootYawOffset = -TurnLimit_Yaw;
		// 이렇게 해도 같은 결과가 나온다. 나중에 문제가 발생하려나?
		RootYawOffset = FMath::ClampAngle(RootYawOffset, -TurnLimit_Yaw, TurnLimit_Yaw);
		LastCurveValue = CurrentCurveValue;
	}
	else
		LastCurveValue = 0.0f;
}
