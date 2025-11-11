// Fill out your copyright notice in the Description page of Project Settings.


#include "SPHud.h"
#include "Blueprint/UserWidget.h"
#include "SPCharacter_Player.h"
#include "SPInventoryComponent.h"
#include "SPMainWidget.h"

ASPHud::ASPHud()
{

}

void ASPHud::BeginPlay()
{
	Super::BeginPlay();
	// Widget »ý¼º
	if (MainWidgetClass != nullptr)
	{
		MainWidget = CreateWidget<USPMainWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}

	ASPCharacter_Player* PlayerCharacter = Cast<ASPCharacter_Player>(GetOwningPawn());

	if (PlayerCharacter != nullptr)
	{
		USPInventoryComponent* InventoryComp = PlayerCharacter->GetInventoryComponent();
		if (InventoryComp != nullptr)
		{
			MainWidget->SetInventoryComponent(InventoryComp);
			InventoryComp->OnInventoryChanged.AddUObject(this, &ASPHud::OnInventoryChanged);
		}
		else
			debugf(TEXT("InventoryComp is Null"));

		PlayerCharacter->OnItemBeginTouched.AddUObject(this, &ASPHud::OnItemBeginTouched);
		PlayerCharacter->OnItemEndTouched.AddUObject(this, &ASPHud::OnItemEndTouched);
		PlayerCharacter->OnWeaponEquipped.AddUObject(this, &ASPHud::OnWeaponEquipped);
	}
}

void ASPHud::OnItemBeginTouched(FString ItemName)
{
	if (MainWidget != nullptr)
	{
		MainWidget->OnItemBeginTouched(ItemName);
	}
}

void ASPHud::OnItemEndTouched()
{
	if (MainWidget != nullptr)
	{
		MainWidget->OnItemEndTouched();
	}
}

void ASPHud::OnInventoryChanged(USPItemData* ItemData)
{
	if (MainWidget != nullptr)
	{
		MainWidget->OnInventoryChanged(ItemData);
	}
}

void ASPHud::OpenInventory()
{
	if (MainWidget != nullptr)
	{
		MainWidget->OpenInventory();
	}
}

void ASPHud::CloseInventory()
{
	if (MainWidget != nullptr)
	{
		MainWidget->CloseInventory();
	}
}

void ASPHud::OnWeaponEquipped(USPItemData* ItemData, int index)
{
	if (MainWidget != nullptr)
	{
		MainWidget->OnWeaponEquipped(ItemData, index);
	}
}
