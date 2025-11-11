// Fill out your copyright notice in the Description page of Project Settings.


#include "SPLauncher.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "SPProjectile.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/HitResult.h"

ASPLauncher::ASPLauncher()
{
	FireType = EFireType::EF_LineTrace;
}

void ASPLauncher::FireLineTrace()
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	
	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector TraceStart = ViewLocation;
	FVector TraceEnd = TraceStart + ViewRotation.Vector() * fFireRange;

	FHitResult Result;
	FCollisionQueryParams CollisionQueryParams;

	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(this->Owner);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Result, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, CollisionQueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f);

	if (bHit)
	{
		UGameplayStatics::ApplyDamage(Result.GetActor(), Damage, Owner->GetInstigatorController(), this, DamageType);
	}
}


