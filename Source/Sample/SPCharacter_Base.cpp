// Fill out your copyright notice in the Description page of Project Settings.


#include "SPCharacter_Base.h"
#include "SPHealthComponent.h"
#include "SPWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "SPInventoryComponent.h"
#include "SPInventoryListWidgetData.h"
#include "SPItemData.h"
#include "TimerManager.h"

// Sets default values
ASPCharacter_Base::ASPCharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<USPHealthComponent>(TEXT("HealthComponent"));

	InventoryComponent = CreateDefaultSubobject<USPInventoryComponent>(TEXT("InventoryComponent"));

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthComponent && IsValid(HealthComponent))
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ASPCharacter_Base::OnTakeDamage);
		HealthComponent->OnDeath.AddDynamic(this, &ASPCharacter_Base::OnDeath);
	}

	/*if (InventoryComponent && IsValid(InventoryComponent))
	{
		InventoryComponent->OnInventoryChanged.AddUObject(this, &ASPCharacter_Base::OnInventoryChanged);
	}*/

	// 메시에 달려있는 무기 Hide
	if (GetMesh())
	{
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	}
}

void ASPCharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void ASPCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPCharacter_Base::OnTakeDamage(float Health, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// 데미지 처리
	debugf(TEXT("Parent TakeDamage %0f"), Damage);
	HandleTakeDamage(Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void ASPCharacter_Base::OnDeath()
{
	// Death 처리
	debugf(TEXT("Parent Death"));
	HandleDeath();
}

float ASPCharacter_Base::GetHealth()
{
	if(HealthComponent == nullptr)
		return 0.0f;

	return HealthComponent->GetHealth();
}

float ASPCharacter_Base::GetMaxHealth()
{
	if (HealthComponent == nullptr)
		return 0.0f;

	return HealthComponent->GetMaxHealth();
}

void ASPCharacter_Base::EquipWeapon(int WeaponIndex)
{
	if (Weapons[WeaponIndex] == nullptr)
		return;
	
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, GetWeaponSocketName(CurrentWeaponIndex));
	}

	Weapons[WeaponIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));

	CurrentWeapon = Weapons[WeaponIndex];
	CurrentWeaponIndex = WeaponIndex;
}

void ASPCharacter_Base::ServerEquipWeapon_Implementation(int WeaponIndex)
{
	EquipWeapon(WeaponIndex);
	ClientEquipWeapon(WeaponIndex);
}

void ASPCharacter_Base::ClientEquipWeapon_Implementation(int WeaponIndex)
{
	EquipWeapon(WeaponIndex);
	CurrentWeaponIndex = WeaponIndex;
}

void ASPCharacter_Base::MulticastEquipWeapon_Implementation(ASPItem* Item)
{
}

void ASPCharacter_Base::UnEquipWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, GetWeaponSocketName(CurrentWeaponIndex));
	}

	CurrentWeapon = nullptr;
	CurrentWeaponIndex = -1;
}

void ASPCharacter_Base::ServerUnEquipWeapon_Implementation()
{
	UnEquipWeapon();
	ClientUnEquipWeapon();
}

void ASPCharacter_Base::ClientUnEquipWeapon_Implementation()
{
	UnEquipWeapon();
}

void ASPCharacter_Base::HandleAddWeapon(USPItemData* NewWeapon)
{
	if (HasAuthority())
		AddWeapon(NewWeapon);
	else
		ServerAddWeapon(NewWeapon);
}

void ASPCharacter_Base::AddWeapon(USPItemData* NewWeapon)
{
	if (NewWeapon == nullptr)
		return;

	if (!NewWeapon->IsWeapon())
		return;

	if (NewWeapon->IsPrimaryWeapon() && IsFullWeapon())
		return;
	
	if (GetWorld() == nullptr)
	{
		return;
	}

	UClass* WeaponClass = NewWeapon->GetItemClass();
	
	if (IsValid(WeaponClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		debugf(TEXT("WeaponClass is %s"), *WeaponClass->GetName());
		ASPWeapon* newWeapon = GetWorld()->SpawnActor<ASPWeapon>(WeaponClass, GetActorLocation() + FVector(50.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);

		if (newWeapon == nullptr)
		{
			debugf(TEXT("new Weapon Is Null"));
			return;
		}

		int TargetIndex = -1;

		if (newWeapon->IsPrimaryWeapon())
		{
			if (Weapons[0] == nullptr)
			{
				TargetIndex = 0;
			}
			else if (Weapons[1] == nullptr)
			{
				TargetIndex = 1;
				
			}
			else
				debugf(TEXT("Try Adding Not Empty Weapon Slot"));
		}
		else
		{
			TargetIndex = (int)newWeapon->GetItemType();
		}

		if (TargetIndex == -1)
			return;

		newWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, GetWeaponSocketName(TargetIndex));
		Weapons[TargetIndex] = newWeapon;

		for (int i = 0; i < MAXWEAPONCNT; i++)
		{
			if (Weapons[i] == nullptr)
				continue;
			debugf(TEXT("Weapons[%d] = %s"), i, *Weapons[i]->GetName());
		}
	}
}

void ASPCharacter_Base::ServerAddWeapon_Implementation(USPItemData* NewWeapon)
{
	AddWeapon(NewWeapon);
	ClientAddWeapon(NewWeapon);
}

void ASPCharacter_Base::ClientAddWeapon_Implementation(USPItemData* NewWeapon)
{
	AddWeapon(NewWeapon);
}

void ASPCharacter_Base::RemoveWepaon(int WeaponIndex)
{
	if (Weapons[WeaponIndex] != nullptr)
	{
		Weapons[WeaponIndex]->Destroy();
	}
}

void ASPCharacter_Base::ChangeWeapon(int WeaponIndex, USPItemData* NewWeapon)
{
	RemoveWepaon(WeaponIndex);
	Weapons[WeaponIndex] = nullptr;
	AddWeapon(NewWeapon);
}

void ASPCharacter_Base::ThrowWeapon(int WeaponIndex)
{
	ASPWeapon* TargetWeapon = nullptr;
	if (Weapons[WeaponIndex] != nullptr)
		TargetWeapon = Weapons[WeaponIndex];
	else if(Weapons[0] != nullptr)
		TargetWeapon = Weapons[0];
	else if (Weapons[1] != nullptr)
		TargetWeapon = Weapons[1];

	if (TargetWeapon == nullptr)
		return;

	FVector ThrowDir = GetActorForwardVector() + FVector(0, 0, 0.2f);
	ThrowDir.Normalize();

	TargetWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	TargetWeapon->Throw(ThrowDir, 100.0f);
}

void ASPCharacter_Base::UseItem(USPItemData* Item)
{
}

FName ASPCharacter_Base::GetWeaponSocketName(int index)
{
	switch (index)
	{
	case 0:
		return TEXT("Primary_Weapon_Socket_l");
	case 1:
		return TEXT("Primary_Weapon_Socket_r");
	case 2:
		return TEXT("Secondary_Weapon_Socket");
	case 3:
		return TEXT("Melee_Weapon_Socket");
	case 4:
		return TEXT("Throwable_Weapon_Socket");
	default:
		return TEXT("None");
	}
}

void ASPCharacter_Base::PickupItem(ASPItem* Item)
{
	if (Item->IsPrimaryWeapon())
	{
		if (IsFullWeapon())
			ChangeWeapon(0, Item->GetItemData());
		else
			HandleAddWeapon(Item->GetItemData());
	}
	else if (Item->IsWeapon())
	{
		InventoryComponent->AddItem(Item->GetItemData());
		HandleAddWeapon(Item->GetItemData());
	}
	else
	{
		InventoryComponent->AddItem(Item->GetItemData());
	}

	Item->HandlePickup();
}

void ASPCharacter_Base::ServerPickupItem_Implementation(ASPItem* Item)
{
	PickupItem(Item);

	MulticastPickupItem(Item);
}

void ASPCharacter_Base::MulticastPickupItem_Implementation(ASPItem* Item)
{
	// 연출용: 애니메이션, 사운드, 이펙트 등
}

void ASPCharacter_Base::OnItemUsed(USPInventoryListWidgetData* ListItemData)
{
	if (ListItemData == nullptr)
		return;

	USPItemData* ItemData = ListItemData->GetItemData();

	if (ItemData == nullptr)
		return;

	if (ItemData->IsWeapon())
	{
		ChangeWeapon((int)ItemData->GetItemType(), ItemData);
	}
	else
		UseItem(ItemData);
}
