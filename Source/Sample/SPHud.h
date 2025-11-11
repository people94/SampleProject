// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SPHud.generated.h"

class USPMainWidget;
class USPItemData;

UCLASS()
class SAMPLE_API ASPHud : public AHUD
{
	GENERATED_BODY()

public:
	ASPHud();
	void BeginPlay();
	
	// Inventory

	void OnItemBeginTouched(FString ItemName);
	void OnItemEndTouched();
	void OnInventoryChanged(USPItemData* ItemData);
	void OpenInventory();
	void CloseInventory();
	void OnWeaponEquipped(USPItemData* ItemData, int Index);

private:
	// MainWidget

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "MainWidget")
	TSubclassOf<USPMainWidget> MainWidgetClass;
	USPMainWidget* MainWidget;
};
