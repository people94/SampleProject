// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SPCharacter_Base.generated.h"

class ASPWeapon;
class ASPItem;
class USPItemData;
class USPInventoryComponent;
class USPInventoryListWidgetData;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquipped, USPItemData*, int);

UCLASS()
class SAMPLE_API ASPCharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPCharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Health

	UFUNCTION()
	void OnTakeDamage(float Health, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	virtual void HandleTakeDamage(float Health, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {}
	
	UFUNCTION()
	void OnDeath();
	virtual void HandleDeath() {}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth();

	// Weapon

	void EquipWeapon(int WeaponIndex);
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(int WeaponIndex);
	UFUNCTION(Client, Reliable)
	void ClientEquipWeapon(int WeaponIndex);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon(ASPItem* Item);
	UFUNCTION()
	void UnEquipWeapon();
	UFUNCTION(Server, Reliable)
	void ServerUnEquipWeapon();
	UFUNCTION(Client, Reliable)
	void ClientUnEquipWeapon();
	void HandleAddWeapon(USPItemData* NewWeapon);
	void AddWeapon(USPItemData* NewWeapon);
	UFUNCTION(Server, Reliable)
	void ServerAddWeapon(USPItemData* NewWeapon);
	UFUNCTION(Client, Reliable)
	void ClientAddWeapon(USPItemData* NewWeapon);
	void RemoveWepaon(int WeaponIndex);
	void ChangeWeapon(int WeaponIndex, USPItemData* NewWeapon);
	void ThrowWeapon(int WeaponIndex);
	void UseItem(USPItemData* Item);
	UFUNCTION(BlueprintPure)
	bool GetIsFiring() { return bIsFiring; }
	bool IsFullWeapon() { return Weapons[0] != nullptr && Weapons[1] != nullptr; }
	FName GetWeaponSocketName(int index);

	// Inventory

	virtual void SetTouchItem(ASPItem* Item) {}
	virtual void PickupItem(ASPItem* Item);
	UFUNCTION(Server, Reliable)
	virtual void ServerPickupItem(ASPItem* Item);
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastPickupItem(ASPItem* Item);
	UFUNCTION(BlueprintPure)
	virtual ASPItem* GetTouchItem() { return TouchItem; }
	UFUNCTION()
	virtual void HandleInventoryChanged() {}
	USPInventoryComponent* GetInventoryComponent() { return InventoryComponent; }
	virtual void OnItemUsed(USPInventoryListWidgetData* ListItemData);

protected:
	// Health

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USPHealthComponent* HealthComponent;

	// Weapon

	bool bIsFiring = false;
	int CurrentWeaponIndex = -1;
	ASPWeapon* Weapons[MAXWEAPONCNT];

	TObjectPtr<ASPWeapon> CurrentWeapon;

	// Inventory

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USPInventoryComponent* InventoryComponent;
	ASPItem* TouchItem;

public:
	FOnWeaponEquipped OnWeaponEquipped;
};