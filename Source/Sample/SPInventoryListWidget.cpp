// Fill out your copyright notice in the Description page of Project Settings.


#include "SPInventoryListWidget.h"
#include "SPInventoryListWidgetData.h"
#include "Components/TextBlock.h"

void USPInventoryListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	AddItemToList(ListItemObject);
}

void USPInventoryListWidget::SetItemName(FString Name)
{
	if (ItemName != nullptr)
	{
		ItemName->SetText(FText::FromString(Name));
	}
}

void USPInventoryListWidget::AddItemToList(UObject* Item)
{
	USPInventoryListWidgetData* ItemData = Cast<USPInventoryListWidgetData>(Item);

	if (ItemData != nullptr)
	{
		SetItemName(ItemData->GetItemName());
	}
}
