// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPMainWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UListView;
class USPItemData;
class USPInventoryComponent;
class UGridPanel;

/**
 * 
 */
UCLASS()
class SAMPLE_API USPMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// Inventory
	void OnItemBeginTouched(FString ItemName);
	void OnItemEndTouched();
	void OnInventoryChanged(USPItemData* ItemData);
	void OnItemUsed(UObject* Item);
	void OpenInventory();
	void CloseInventory();
	void SetInventoryComponent(USPInventoryComponent* InvComp);
	void OnWeaponEquipped(USPItemData* ItemData, int Index);

protected:
	// Widget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBarWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CrosshairWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNoticeWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> InventoryListWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> WeaponSlotWidget;

private:
	TObjectPtr<USPInventoryComponent> InventoryComponent;
};