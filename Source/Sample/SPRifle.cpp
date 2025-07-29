// Fill out your copyright notice in the Description page of Project Settings.


#include "SPRifle.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "SPProjectile.h"

ASPRifle::ASPRifle()
{
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

	Projectile = GetWorld()->SpawnActor<ASPProjectile>(ProjectileClass, ProjectileSpawnPosition->GetComponentLocation(), Owner ? Owner->GetActorRotation() : FRotator(0, 0, 0));
}