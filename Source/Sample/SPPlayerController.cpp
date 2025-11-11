// Fill out your copyright notice in the Description page of Project Settings.


#include "SPPlayerController.h"
#include "Engine/World.h"
#include "SPCharacter_Player.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "SPHud.h"
#include "SPInventoryComponent.h"

ASPPlayerController::ASPPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent != nullptr)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhancedInputComponent == nullptr) return;

		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleMoveInput);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleJumpInput);
		EnhancedInputComponent->BindAction(RotateInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleRotateInput);
		EnhancedInputComponent->BindAction(LockCharacterTurnInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleToggleLockInput);
		EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleCrouchInput);
		EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Started, this, &ASPPlayerController::HandleFireInput);
		EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Completed, this, &ASPPlayerController::HandleFireInput);
		EnhancedInputComponent->BindAction(InteractionInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleInteractionInput);
		EnhancedInputComponent->BindAction(InventoryInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleInventoryInput);
		EnhancedInputComponent->BindAction(WeaponSelectInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleWeaponSelectInput);
		EnhancedInputComponent->BindAction(UnEquipWeaponInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleUnEquipWeaponInput);
		EnhancedInputComponent->BindAction(ThrowWeaponInputAction, ETriggerEvent::Triggered, this, &ASPPlayerController::HandleThrowWeaponInput);
	}
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
}

void ASPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Pawn = Cast<ASPCharacter_Player>(InPawn);
	if (Pawn)
	{
		InventoryComponent = Pawn->GetInventoryComponent();
		if (InventoryComponent)
		{
			InventoryComponent->OnItemUsed.AddUObject(this, &ASPPlayerController::OnItemUsed);
		}
	}
}

void ASPPlayerController::HandleMoveInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;
	
	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;
	
	FVector InputVector = Instance.GetValue().Get<FVector>();
	PlayerPawn->MoveCharacter(InputVector);
}

void ASPPlayerController::HandleJumpInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;

	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	PlayerPawn->Jump();
}

void ASPPlayerController::HandleRotateInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;
	
	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	FVector InputVector = Instance.GetValue().Get<FVector>();
	PlayerPawn->RotateCharacter(InputVector);
}

void ASPPlayerController::HandleToggleLockInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;

	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	bool InputValue = Instance.GetValue().Get<bool>();
	PlayerPawn->ToggleLockCharacterTurn(InputValue);
}

void ASPPlayerController::HandleCrouchInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;
	
	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	bool InputValue = Instance.GetValue().Get<bool>();
	PlayerPawn->StartCrouch(InputValue);
}

void ASPPlayerController::HandleFireInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;
	
	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	bool InputValue = Instance.GetValue().Get<bool>();
	if(InputValue)
		PlayerPawn->StartFire();
	else
		PlayerPawn->StopFire();
}

void ASPPlayerController::HandleInteractionInput(const FInputActionInstance& Instance)
{

	if (GetPawn() == nullptr)
		return;

	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	bool InputValue = Instance.GetValue().Get<bool>();
	PlayerPawn->HandleInteraction();
}

void ASPPlayerController::HandleInventoryInput(const FInputActionInstance& Instance)
{
	bool InputValue = Instance.GetValue().Get<bool>();

	ASPHud* MyHud = Cast<ASPHud>(GetHUD());

	if (MyHud == nullptr)
		return;

	if (InputValue)
	{
		SetUIInputMode();
		MyHud->OpenInventory();
	}
	else
	{
		SetIngameInputMode();
		MyHud->CloseInventory();
	}
}

void ASPPlayerController::HandleWeaponSelectInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;

	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	int TriggerKey = (int)Instance.GetValue().Get<float>();

	PlayerPawn->HandleWeaponSelect(TriggerKey);
}

void ASPPlayerController::HandleUnEquipWeaponInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;

	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	PlayerPawn->HandleUnEquipWeapon();
}

void ASPPlayerController::HandleThrowWeaponInput(const FInputActionInstance& Instance)
{
	if (GetPawn() == nullptr)
		return;

	ASPCharacter_Player* PlayerPawn = Cast<ASPCharacter_Player>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	PlayerPawn->ThrowWeapon(0);
}

void ASPPlayerController::SetUIInputMode()
{
	// 마우스 커서 보여주기.
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	// 마우스가 Viewport 화면 밖으로 나가게 할지 아닌지 결정. LockAlways 하면 화면밖으로 마우스를 내보낼 수 없음.
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	// 마우스 클릭중 커서 UI에서 안숨긴다
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
}

void ASPPlayerController::SetIngameInputMode()
{
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ASPPlayerController::OnItemUsed(USPInventoryListWidgetData* ItemData)
{
	if (Pawn != nullptr)
	{
		Pawn->OnItemUsed(ItemData);
	}
}
