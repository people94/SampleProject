// Fill out your copyright notice in the Description page of Project Settings.


#include "SPProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASPProjectile::ASPProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
		Root->SetAbsolute(false, false, false);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(GetRootComponent());
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = 2000.0f;
		ProjectileMovementComponent->MaxSpeed = 2000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

		ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	}
}

// Called when the game starts or when spawned
void ASPProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

