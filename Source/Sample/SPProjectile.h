// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPProjectile.generated.h"

UCLASS()
class SAMPLE_API ASPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
};
