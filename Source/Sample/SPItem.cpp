// Fill out your copyright notice in the Description page of Project Settings.


#include "SPItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SPCharacter_Player.h"
#include "SPItemData.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ASPItem::ASPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
	}

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	if (TriggerComponent)
	{
		TriggerComponent->SetupAttachment(GetRootComponent());
	}

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(GetRootComponent());
	}
}

// Called when the game starts or when spawned
void ASPItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAnyFlags(RF_WasLoaded))
		bCanBePickedUp = true;
	else
		bCanBePickedUp = false;

	if (TriggerComponent != nullptr)
	{
		if (bCanBePickedUp)
		{
			TriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
			TriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
		if (!TriggerComponent->OnComponentBeginOverlap.IsAlreadyBound(this, &ASPItem::OnBeginOverlap))
		{
			TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ASPItem::OnBeginOverlap);
		}

		if (!TriggerComponent->OnComponentEndOverlap.IsAlreadyBound(this, &ASPItem::OnEndOverlap))
		{
			TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ASPItem::OnEndOverlap);
		}
	}

	if (Mesh != nullptr)
	{
		if (bCanBePickedUp)
		{
			debugf(TEXT("2222222222222222222222"));
			Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Mesh->SetSimulatePhysics(true);
			Mesh->SetAllBodiesSimulatePhysics(true);
			Mesh->SetNotifyRigidBodyCollision(true);
			Mesh->WakeAllRigidBodies();
		}

		if (!Mesh->OnComponentHit.IsAlreadyBound(this, &ASPItem::OnHit))
		{
			debugf(TEXT("3333333333333333333333"));
			Mesh->OnComponentHit.AddDynamic(this, &ASPItem::OnHit);
		}
	}

	if (ItemData != nullptr)
	{
		ItemData->SetItemClass(this->GetClass());
	}
}

// Called every frame
//void ASPItem::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

USkeletalMeshComponent* ASPItem::GetMesh()
{
	return Mesh;
}

void ASPItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASPItem::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	debugf(TEXT("ASPItem::HandleBeginOverlap"));
	if (OtherActor != nullptr)
	{
		ASPCharacter_Player* Player = Cast<ASPCharacter_Player>(OtherActor);
		if (Player != nullptr)
		{
			Player->SetTouchItem(this);
		}
	}
}

void ASPItem::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HandleEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}

void ASPItem::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	debugf(TEXT("ASPItem::HandleEndOverlap"));
	if (OtherActor != nullptr)
	{
		ASPCharacter_Player* Player = Cast<ASPCharacter_Player>(OtherActor);
		Player->SetTouchItem(nullptr);
	}
}

void ASPItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HandleHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ASPItem::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp != nullptr && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		if (Mesh != nullptr)
		{
			bCanBePickedUp = true;
			Mesh->SetLinearDamping(5.0f);
			Mesh->SetAngularDamping(10.0f);
			Mesh->SetNotifyRigidBodyCollision(false);

			GetWorld()->GetTimerManager().SetTimer(EndSimulateTimerHandle, [this]()
				{
					Mesh->SetSimulatePhysics(false);
					Mesh->SetAllBodiesSimulatePhysics(false);
					Mesh->SetCollisionProfileName(TEXT("NoCollision"));
				}, 0.5f, false);
		}
		
		if (TriggerComponent != nullptr)
		{
			TriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ASPItem::HandlePickup()
{
	if(HasAuthority())
		Destroy();
}

FString ASPItem::GetItemName()
{
	return ItemData ? ItemData->GetItemName() : GetName();
}

EItemType ASPItem::GetItemType()
{
	return ItemData ? ItemData->GetItemType() : EItemType::None;
}

bool ASPItem::IsWeapon()
{
	if (ItemData == nullptr)
		return false;

	return ItemData->IsWeapon();
}

bool ASPItem::IsPrimaryWeapon()
{
	if (ItemData == nullptr)
		return false;

	return ItemData->IsPrimaryWeapon();
}

void ASPItem::Throw(FVector ThrowDir, float Speed)
{
	if (Mesh)
	{
		Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
		Mesh->SetAllBodiesSimulatePhysics(true);
		Mesh->SetNotifyRigidBodyCollision(true);
		Mesh->WakeAllRigidBodies();

		Mesh->AddImpulse(ThrowDir * Speed, NAME_None, true);
	}
}

void ASPItem::FinishThrow()
{
	if (TriggerComponent != nullptr)
	{
		TriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ASPItem::EnablePickup()
{
	bCanBePickedUp = true;
	if (TriggerComponent != nullptr)
	{
		TriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ASPItem::DisablePickup()
{
	bCanBePickedUp = false;
	if (TriggerComponent != nullptr)
	{
		TriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
