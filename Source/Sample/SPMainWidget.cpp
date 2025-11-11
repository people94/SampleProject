// Fill out your copyright notice in the Description page of Project Settings.

#include "SPMainWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "SPInventoryListWidgetData.h"
#include "SPItem.h"
#include "SPItemData.h"
#include "SPInventoryComponent.h"

void USPMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (InventoryListWidget != nullptr)
	{
		InventoryListWidget->OnItemDoubleClicked().AddUObject(this, &USPMainWidget::OnItemUsed);
	}
}

void USPMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemNoticeWidget != nullptr)
		ItemNoticeWidget->SetVisibility(ESlateVisibility::Hidden);
}

void USPMainWidget::OnItemBeginTouched(FString ItemName)
{
	if (ItemNoticeWidget != nullptr)
	{
		ItemNoticeWidget->SetText(FText::FromString(ItemName));
		ItemNoticeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USPMainWidget::OnItemEndTouched()
{
	if (ItemNoticeWidget != nullptr)
	{
		ItemNoticeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USPMainWidget::OnInventoryChanged(USPItemData* ItemData)
{
	if (InventoryListWidget != nullptr)
	{
		USPInventoryListWidgetData* WidgetItemData = NewObject<USPInventoryListWidgetData>();
		if(WidgetItemData)
		{
			WidgetItemData->SetItemData(ItemData);
			InventoryListWidget->AddItem(WidgetItemData);
		}
	}
}

void USPMainWidget::OnItemUsed(UObject* Item)
{
	if (Item == nullptr)
		return;

	USPInventoryListWidgetData* ListItemData = Cast<USPInventoryListWidgetData>(Item);
	
	if (InventoryComponent != nullptr)
	{
		InventoryComponent->OnItemUsed.Broadcast(ListItemData);
	}
	if (InventoryListWidget != nullptr)
	{
		InventoryListWidget->RemoveItem(Item);
	}
}

void USPMainWidget::OpenInventory()
{
	if (InventoryListWidget != nullptr)
	{
		InventoryListWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (WeaponSlotWidget != nullptr)
	{
		WeaponSlotWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USPMainWidget::CloseInventory()
{
	if (InventoryListWidget != nullptr)
	{
		InventoryListWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WeaponSlotWidget != nullptr)
	{
		WeaponSlotWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USPMainWidget::SetInventoryComponent(USPInventoryComponent* InvComp)
{
	if (InvComp != nullptr)
	{
		InventoryComponent = InvComp;
	}
}

void USPMainWidget::OnWeaponEquipped(USPItemData* ItemData, int index)
{
	if (WeaponSlotWidget == nullptr)
		return;

	if (ItemData == nullptr)
		return;

	UTextBlock* NewTextBlock = NewObject<UTextBlock>(this);
	NewTextBlock->SetText(FText::FromString(ItemData->GetItemName()));
	UGridSlot* NewSlot = WeaponSlotWidget->AddChildToGrid(NewTextBlock);

	if (NewSlot == nullptr)
		return;
	
	if (ItemData->IsPrimaryWeapon())
	{
		NewSlot->SetRow(index);
		NewSlot->SetPadding(FMargin(0.f, 50.f, 0.f, 50.f));
	}
	else
	{
		NewSlot->SetRow(index);
		NewSlot->SetPadding(FMargin(0.f, 25.f, 0.f, 25.f));
	}
}

