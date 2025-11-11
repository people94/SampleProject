// Fill out your copyright notice in the Description page of Project Settings.


#include "SPCharacter_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SPWeapon.h"
#include "SPAnimInstance_Character.h"
#include "Kismet/GameplayStatics.h"
#include "SPCharacter_Base.h"
#include "SPInventoryComponent.h"
#include "SPItem.h"
#include "SPItemData.h"

ASPCharacter_Player::ASPCharacter_Player()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	bReplicates = true;
}

void ASPCharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPCharacter_Player::BeginPlay()
{
	Super::BeginPlay();

	//AddWeapon(TEXT("/Game/Weapon/BP_Rifle.BP_Rifle_C"));
	//AddWeapon(TEXT("/Game/Weapon/BP_Launcher.BP_Launcher_C"));
	//ChangeWeapon(0);
}

void ASPCharacter_Player::MoveCharacter(const FVector InputVector)
{
	FVector MoveDirection = this->GetActorForwardVector() * InputVector.X + GetActorRightVector() * InputVector.Y;

	float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this); // World DeltaSeconds 반영

	if (GetMovementComponent())
	{
		this->AddMovementInput(MoveDirection * GetMovementComponent()->GetMaxSpeed() * WorldDeltaSeconds);
	}
	double RotateAmount = FMath::FInterpConstantTo(GetActorRotation().Yaw, Camera->GetComponentRotation().Yaw, UGameplayStatics::GetWorldDeltaSeconds(this), TurnRate);

	if(bLockCharacterTurn)
		SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
	else
		SetActorRotation(FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0f));
}

void ASPCharacter_Player::RotateCharacter(const FVector InputVector)
{
	FRotator Rotator(0.0f, InputVector.X, 0.0f);
	float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this); // World DeltaSeconds 반영

	AddControllerYawInput(InputVector.X * TurnRate * WorldDeltaSeconds); // Pitch 회전
	AddControllerPitchInput(-InputVector.Y * TurnRate * WorldDeltaSeconds); // Pitch 회전
}

void ASPCharacter_Player::ToggleLockCharacterTurn(const bool InputValue)
{
	bLockCharacterTurn = InputValue;
	UE_LOG(LogTemp, Warning, TEXT("bLockCharacterTurn = %d"), bLockCharacterTurn);
	if (!bLockCharacterTurn)
	{
		SpringArm->SetRelativeRotation(GetActorRotation());
		GetController()->SetControlRotation(GetActorRotation());
	}
}

void ASPCharacter_Player::StartCrouch(const bool InputValue)
{
	if (InputValue)
		Crouch();
	else
		UnCrouch();

	UE_LOG(LogTemp, Warning, TEXT("Is Crouching : %d"), bIsCrouched);
}

void ASPCharacter_Player::StartFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire OnGoing"));
	bIsFiring = true;
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}

	UGameplayStatics::ApplyDamage(this, 10, nullptr, nullptr, nullptr);
}

void ASPCharacter_Player::StopFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Stop"));
	bIsFiring = false;
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ASPCharacter_Player::HandleInteraction()
{
	if (TouchItem)
	{
		ServerPickupItem(TouchItem);
	}
}

void ASPCharacter_Player::HandleWeaponSelect(int SelectIndex)
{
	ServerEquipWeapon(SelectIndex - 1);
}

void ASPCharacter_Player::HandleUnEquipWeapon()
{
	ServerUnEquipWeapon();
}

void ASPCharacter_Player::SetTouchItem(ASPItem* Item)
{
	if (Item != nullptr)
	{
		TouchItem = Item;
		OnItemBeginTouched.Broadcast(Item->GetItemName());
	}
	else
	{
		TouchItem = nullptr;
		OnItemEndTouched.Broadcast();
	}
}