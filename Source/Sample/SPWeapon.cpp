// Fill out your copyright notice in the Description page of Project Settings.


#include "SPWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"


// Sets default values
ASPWeapon::ASPWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if(Root)
	{
		SetRootComponent(Root);
	}

	ProjectileSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectilePosition"));
	if (ProjectileSpawnPosition)
	{
		ProjectileSpawnPosition->SetupAttachment(GetRootComponent());
	}

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(GetRootComponent());
	}
}

void ASPWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bIsFiring)
	//{
	//	float FireTimeDiff = GetWorld()->GetTimeSeconds() - fLastFireTime;
	//	if (FireTimeDiff > fFireInterval)
	//	{
	//		fLastFireTime = GetWorld()->GetTimeSeconds();
	//		HandleFire();
	//	}
	//}
}

void ASPWeapon::StartFire()
{
	bIsFiring = true;
	HandleFire();
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(FireTimerHandle, this, &ASPWeapon::HandleFire, fFireInterval, true);
}

void ASPWeapon::StopFire()
{
	bIsFiring = false;
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearTimer(FireTimerHandle);
}

void ASPWeapon::HandleFire()
{
	if (!bIsFiring)
		return;

	switch (FireType)
	{
	case EFireType::EF_LineTrace:
		FireLineTrace();
		break;
	case EFireType::EF_Projectile:
		FireProjectile();
		break;
	}
}