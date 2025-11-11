// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPInventoryComponent.generated.h"

class ASPCharacter_Base;
class USPItemData;
class USPInventoryListWidgetData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, USPItemData*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUsed, USPInventoryListWidgetData*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLE_API USPInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPInventoryComponent();

	int GetItemsNum() { return Items.Num(); }
	USPItemData* GetItem(const int Index);
	void AddItem(USPItemData* ItemData);
	UFUNCTION(Client, Reliable)
	void ClientUpdateInventory(USPItemData* ItemData);
	void UseItem();
	void HandleEquipWeapon(USPItemData* ItemData, int Index);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	
	TArray< TObjectPtr<USPItemData> > Items;
	USPItemData* Weapons[MAXWEAPONCNT];

public:
	FOnInventoryChanged OnInventoryChanged;
	TObjectPtr<ASPCharacter_Base> OwnerCharacter;
	FOnItemUsed OnItemUsed;
};
