// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleCharacter_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SPWeapon.h"
#include "SampleAnimInstance_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ASampleCharacter_Player::ASampleCharacter_Player()
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

void ASampleCharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASampleCharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASampleCharacter_Player::MoveCharacter);
	EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(RotateInputAction, ETriggerEvent::Triggered, this, &ASampleCharacter_Player::RotateCharacter);
	EnhancedInputComponent->BindAction(LockCharacterTurnInputAction, ETriggerEvent::Triggered, this, &ASampleCharacter_Player::ToggleLockCharacterTurn);
	EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Triggered, this, &ASampleCharacter_Player::StartCrouch);
	EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Started, this, &ASampleCharacter_Player::StartFire);
	EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Completed, this, &ASampleCharacter_Player::StopFire);
}

void ASampleCharacter_Player::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr) return;

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}

	// 무기 생성
	if (GetMesh())
	{
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	}
	AddWeapon(TEXT("/Game/Weapon/BP_Rifle.BP_Rifle_C"));
	AddWeapon(TEXT("/Game/Weapon/BP_Launcher.BP_Launcher_C"));
	ChangeWeapon(0);


	// Widget 생성
	if (WidgetClass != nullptr)
	{
		PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (PlayerWidget)
		{
			PlayerWidget->AddToViewport();
		}
	}
	
}

void ASampleCharacter_Player::MoveCharacter(const FInputActionInstance& Instance)
{
	FVector InputVector = Instance.GetValue().Get<FVector>();
	FVector MoveDirection = this->GetActorForwardVector() * InputVector.X + GetActorRightVector() * InputVector.Y;

	float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this); // World DeltaSeconds 반영

	this->AddMovementInput(MoveDirection * WalkSpeed * WorldDeltaSeconds);
	double RotateAmount = FMath::FInterpConstantTo(GetActorRotation().Yaw, Camera->GetComponentRotation().Yaw, UGameplayStatics::GetWorldDeltaSeconds(this), TurnRate);

	if(bLockCharacterTurn)
		SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
	else
		SetActorRotation(FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0f));
}

void ASampleCharacter_Player::RotateCharacter(const FInputActionInstance& Instance)
{
	FVector InputVector = Instance.GetValue().Get<FVector>();
	FRotator Rotator(0.0f, InputVector.X, 0.0f);
	float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this); // World DeltaSeconds 반영

	AddControllerYawInput(InputVector.X * TurnRate * WorldDeltaSeconds); // Pitch 회전
	AddControllerPitchInput(-InputVector.Y * TurnRate * WorldDeltaSeconds); // Pitch 회전
}

void ASampleCharacter_Player::ToggleLockCharacterTurn(const FInputActionInstance& Instance)
{
	bLockCharacterTurn = Instance.GetValue().Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("bLockCharacterTurn = %d"), bLockCharacterTurn);
	if (!bLockCharacterTurn)
	{
		SpringArm->SetRelativeRotation(GetActorRotation());
		GetController()->SetControlRotation(GetActorRotation());
	}
}

void ASampleCharacter_Player::StartCrouch(const FInputActionInstance& Instance)
{
	if (Instance.GetValue().Get<bool>())
		Crouch();
	else
		UnCrouch();

	UE_LOG(LogTemp, Warning, TEXT("Is Crouching : %d"), bIsCrouched);
}

void ASampleCharacter_Player::StartFire(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire OnGoing"));
	bIsFiring = Instance.GetValue().Get<bool>();
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ASampleCharacter_Player::StopFire(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Stop"));
	bIsFiring = Instance.GetValue().Get<bool>();
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ASampleCharacter_Player::AddWeapon(const FString WeapClassName)
{
	if(WeapClassName == TEXT(""))
		return;

	if (GetWorld() == nullptr)
	{
		return;
	}

	UClass * WeaponClass = LoadClass<ASPWeapon>(nullptr, *WeapClassName);

	if (WeaponClass)
	{
		ASPWeapon* newWeapon = GetWorld()->SpawnActor<ASPWeapon>(WeaponClass);
		Weapons.Add(newWeapon);
	}
}

void ASampleCharacter_Player::ChangeWeapon(const int WeaponIndex)
{
	if (Weapons.Num() == 0 || WeaponIndex > Weapons.Num() - 1)
		return;

	CurrentWeaponIndex = WeaponIndex;
	if (Weapons[CurrentWeaponIndex])
	{
		Weapons[CurrentWeaponIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		CurrentWeapon = Weapons[CurrentWeaponIndex];
	}
	else
		debugf(TEXT("Weapons[%d] is Empty!!!"), CurrentWeaponIndex);
}
