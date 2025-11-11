// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "SPPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;
class ASPHud;
class USPInventoryComponent;
class ASPCharacter_Player;
class USPInventoryListWidgetData;

/**
 * 
 */
UCLASS()
class SAMPLE_API ASPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPPlayerController();

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

	// Character

	virtual void OnPossess(APawn* InPawn) override;

	// Input

	virtual void HandleMoveInput(const FInputActionInstance& Instance);

	virtual void HandleJumpInput(const FInputActionInstance& Instance);

	virtual void HandleRotateInput(const FInputActionInstance& Instance);

	void HandleToggleLockInput(const FInputActionInstance& Instance);

	void HandleCrouchInput(const FInputActionInstance& Instance);

	void HandleFireInput(const FInputActionInstance& Instance);

	void HandleInteractionInput(const FInputActionInstance& Instance);

	void HandleInventoryInput(const FInputActionInstance& Instance);

	void HandleWeaponSelectInput(const FInputActionInstance& Instance);

	void HandleUnEquipWeaponInput(const FInputActionInstance& Instance);

	void HandleThrowWeaponInput(const FInputActionInstance& Instance);

	void SetUIInputMode();

	void SetIngameInputMode();

	// Inventory

	void OnItemUsed(USPInventoryListWidgetData* ItemData);

protected:

	//Charcter

	TObjectPtr<ASPCharacter_Player> Pawn;

	// Inventory

	TObjectPtr<USPInventoryComponent> InventoryComponent;
	
	// Input

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RotateInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LockCharacterTurnInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractionInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InventoryInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> WeaponSelectInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> UnEquipWeaponInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ThrowWeaponInputAction;
};
