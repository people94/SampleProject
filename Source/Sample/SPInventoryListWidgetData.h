// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SPItemData.h"
#include "SPInventoryListWidgetData.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API USPInventoryListWidgetData : public UObject
{
	GENERATED_BODY()
	
public:
	FString GetItemName() {	return ItemData ? ItemData->GetItemName() : TEXT("None"); }
	void SetItemData(USPItemData* NewItemData) { ItemData = NewItemData; }
	USPItemData* GetItemData() { return ItemData; }

private:
	TObjectPtr<USPItemData> ItemData;
};
