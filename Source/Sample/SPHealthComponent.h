// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnHealthChangedSignature,
	float, Health,
	float, DamageAmount,
	const class UDamageType*, DamageType,
	class AController*, InstigatedBy,
	AActor*, DamageCauser
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDealthSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLE_API USPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, meta = ( AllowPrivateAccess = "true" ), Category = "Health")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float Health = 0.0f;
		
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDealthSignature OnDeath;

	// Get & Set
	UFUNCTION(BlueprintPure)
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth) { Health = NewHealth; }
	
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	
	UFUNCTION()
	void TakeDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
