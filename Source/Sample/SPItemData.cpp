// Fill out your copyright notice in the Description page of Project Settings.


#include "SPItemData.h"

bool USPItemData::IsWeapon()
{
	switch (GetItemType())
	{
	case EItemType::PrimaryWeapon:
	case EItemType::SecondaryWeapon:
	case EItemType::MeleeWeapon:
	case EItemType::ThrowableWeapon:
		return true;
	default:
		return false;
	}
}

bool USPItemData::IsPrimaryWeapon()
{
	switch (GetItemType())
	{
	case EItemType::PrimaryWeapon:
		return true;
	default:
		return false;
	}
}
