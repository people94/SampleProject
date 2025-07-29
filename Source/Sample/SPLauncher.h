// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPWeapon.h"
#include "SPLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ASPLauncher : public ASPWeapon
{
	GENERATED_BODY()
	
public:
	ASPLauncher();

protected:
	virtual void FireLineTrace() override;
};
