// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPWeapon.h"
#include "SPRifle.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ASPRifle : public ASPWeapon
{
	GENERATED_BODY()

public:
	ASPRifle();
	
public:

	virtual void FireProjectile() override;
};
