// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SPInventoryListWidget.generated.h"

class UTextBlock;
class ASPItem;

/**
 * 
 */
UCLASS()
class SAMPLE_API USPInventoryListWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void SetItemName(FString Name);

	void AddItemToList(UObject* Item);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
};
