// Fill out your copyright notice in the Description page of Project Settings.


#include "SPProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASPProjectile::ASPProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	if (CollisionComponent)
	{
		CollisionComponent->InitSphereRadius(CollisionRadius);
		CollisionComponent->SetNotifyRigidBodyCollision(true);
		SetRootComponent(CollisionComponent);
	
		CollisionComponent->OnComponentHit.AddDynamic(this, &ASPProjectile::OnHit);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASPProjectile::OnBeginOverlap);
	}
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(GetRootComponent());
	}
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	}
}

void ASPProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//debugf(TEXT("Parent OnHit"));
	HandleHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	Destroy();
}

void ASPProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//debugf(TEXT("Parent OnBeginOverlap"));
	HandleBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	Destroy();
}
