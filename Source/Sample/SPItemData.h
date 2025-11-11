// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SPItem.h"
#include "SPItemData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLE_API USPItemData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FString GetItemName() { return ItemName != TEXT("") ? ItemName : GetName(); }
	EItemType GetItemType() { return ItemType; }
	void SetItemClass(UClass* NewItemClass) { ItemClass = NewItemClass; }
	UClass* GetItemClass() const  { return ItemClass; }
	bool IsWeapon();
	bool IsPrimaryWeapon();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TSubclassOf<ASPItem> ItemClass;
};
