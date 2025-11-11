// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPItem.generated.h"

class ASPWeapon;
class USkeletalMeshComponent;
class UBoxComponent;
class USPItemData;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None	UMETA(DisplayName = "None"),
	PrimaryWeapon	UMETA(DisplayName = "Primary Weapon"),
	SecondaryWeapon	UMETA(DisplayName = "Secondary Weapon"),
	MeleeWeapon	UMETA(DisplayName = "Melee Weapon"),
	ThrowableWeapon	UMETA(DisplayName = "Throwable Weapon"),
	Health	UMETA(DisplayName = "Health Item"),
	Stamina	UMETA(DisplayName = "Stamina Item"),
};

UCLASS()
class SAMPLE_API ASPItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Mesh

	USkeletalMeshComponent* GetMesh();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex);

	virtual void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HandlePickup();

	FString GetItemName();

	USPItemData* GetItemData() { return ItemData; }

	EItemType GetItemType();

	bool IsWeapon();
	bool IsPrimaryWeapon();

	void Throw(FVector ThrowDir, float Speed);
	void FinishThrow();
	void EnablePickup();
	void DisablePickup();

private:
	// Mesh

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Root")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Volume")
	TObjectPtr<UBoxComponent> TriggerComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Item")
	TObjectPtr<USPItemData> ItemData;

	bool bCanBePickedUp = false;
	FTimerHandle EndSimulateTimerHandle;
};
