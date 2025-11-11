// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPProjectile.h"
#include "SPBullet.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ASPBullet : public ASPProjectile
{
	GENERATED_BODY()

public:
	ASPBullet();
	
	virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPricateAccess="true"), Category = "Damage")
	float DamageAmp = 1.0f;
	
	
};
