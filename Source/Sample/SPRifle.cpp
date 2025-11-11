// Fill out your copyright notice in the Description page of Project Settings.


#include "SPRifle.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "SPProjectile.h"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"

ASPRifle::ASPRifle()
{
	FireType = EFireType::EF_Projectile;

	ProjectileSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectilePosition"));
	if (ProjectileSpawnPosition)
	{
		ProjectileSpawnPosition->SetupAttachment(GetRootComponent());
	}
}

void ASPRifle::FireProjectile()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	if (ProjectileClass == nullptr)
	{
		return;
	}

	if (ProjectileSpawnPosition == nullptr)
	{
		return;
	}
	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewLocation, ViewRotation); // 카메라의 위치와 회전
	
	FVector TraceStart = ViewLocation;
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * fFireRange;

	DrawDebugSphere(GetWorld(), TraceStart, 32.0f, 16, FColor::Green, false, 5.0f);

	FHitResult Result;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);
	CollisionParams.AddIgnoredActor(this);
	bool bHit =	GetWorld()->LineTraceSingleByChannel(Result, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams); // TraceStart 에서 TraceEnd 까지 보이지 않는 선을 그리는것.
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 5.0f);

	FVector TargetPoint = bHit ? Result.ImpactPoint : TraceEnd; // 맞았으면 맞춘 쪽으로 발사, 맞지 않았으면 그냥 카메라 방향으로 발사
	FVector FireDirection = (TargetPoint - ProjectileSpawnPosition->GetComponentLocation()).GetSafeNormal();
	DrawDebugLine(GetWorld(), ProjectileSpawnPosition->GetComponentLocation(), TargetPoint, FColor::Red, false, 5.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	Projectile = GetWorld()->SpawnActor<ASPProjectile>(ProjectileClass, ProjectileSpawnPosition->GetComponentLocation(), FireDirection.Rotation(), SpawnParams);
}