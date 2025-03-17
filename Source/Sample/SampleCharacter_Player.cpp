// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleCharacter_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPWeapon.h"

ASampleCharacter_Player::ASampleCharacter_Player()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// 컨트롤러 방향대로 폰을 움직이지 않는다.
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
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
	EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Triggered, this, &ASampleCharacter_Player::StartFire);
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
}

void ASampleCharacter_Player::MoveCharacter(const FInputActionInstance& Instance)
{
	FVector InputVector = Instance.GetValue().Get<FVector>();
	FVector MoveDirection = this->GetActorForwardVector() * InputVector.X + GetActorRightVector() * InputVector.Y;

	float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this); // World DeltaSeconds 반영

	this->AddMovementInput(MoveDirection * WalkSpeed * WorldDeltaSeconds);
	SetActorRotation(FRotator(0.0f, FMath::FInterpConstantTo(GetActorRotation().Yaw, Camera->GetComponentRotation().Yaw, UGameplayStatics::GetWorldDeltaSeconds(this), TurnRate), 0.0f));
}

void ASampleCharacter_Player::RotateCharacter(const FInputActionInstance& Instance)
{
	FVector InputVector = Instance.GetValue().Get<FVector>();
	FRotator Rotator(0.0f, InputVector.X, 0.0f);
	float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this); // World DeltaSeconds 반영

	SpringArm->AddRelativeRotation(Rotator * TurnRate * WorldDeltaSeconds); // Yaw 회전
	if (!bLockCharacterTurn)
	{
		float CameraYaw = Camera->GetComponentRotation().Yaw;
		float CharacterYaw = GetActorRotation().Yaw;
		if (FMath::Abs<float>(CameraYaw - CharacterYaw) >= TurnLimit_Yaw)
		{
			SetActorRotation(FRotator(0.0f, CameraYaw, 0.0f)); // SpringArm의 Yaw 을 사용하면 캐릭터가 회전할때 SpringArm의 Rotation이 같이 바뀌어서 캐릭터의 회전이 이상해진다.
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetController()->GetControlRotation().ToString());
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
	if (Weapon)
	{
		Weapon->StartFire();
	}
}
