// Fill out your copyright notice in the Description page of Project Settings.


#include "SPInventoryComponent.h"
#include "SPItemData.h"
#include "Engine/World.h"
#include "SPCharacter_Base.h"


// Sets default values for this component's properties
USPInventoryComponent::USPInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		OwnerCharacter = Cast<ASPCharacter_Base>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->OnWeaponEquipped.AddUObject(this, &USPInventoryComponent::HandleEquipWeapon);
		}
	}
}

USPItemData* USPInventoryComponent::GetItem(const int Index)
{
	return nullptr;
}

void USPInventoryComponent::AddItem(USPItemData* ItemData)
{
	if (!ItemData)
		return;

	Items.Add(ItemData);
	ClientUpdateInventory(ItemData);
}

void USPInventoryComponent::ClientUpdateInventory_Implementation(USPItemData* ItemData)
{
	if (!ItemData)
		return;

	OnInventoryChanged.Broadcast(ItemData);
}

void USPInventoryComponent::UseItem()
{
}

void USPInventoryComponent::HandleEquipWeapon(USPItemData* ItemData, int Index)
{
	Weapons[Index] = ItemData;
}
