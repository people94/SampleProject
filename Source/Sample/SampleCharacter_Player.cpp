// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleCharacter_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

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
	//EnhancedInputComponent->BindAction(RotateInputAction, ETriggerEvent::Triggered, this, &APawn::AddControllerYawInput);
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
	FVector2D InputVector2D = Instance.GetValue().Get<FVector2D>();

	FVector MoveDirection(InputVector2D.X, InputVector2D.Y, 0.0f);
	
	this->AddMovementInput(MoveDirection * WalkSpeed);
}

void ASampleCharacter_Player::RotateCharacter(const FInputActionInstance& Instance)
{
	FVector2D InputVector2D = Instance.GetValue().Get<FVector2D>();
	FVector InputVector = Instance.GetValue().Get<FVector>();
	FRotator Rotator = GetActorRotation();
	Rotator.Yaw = InputVector.X;

	AddControllerYawInput(InputVector.X * 50.0f);

	UE_LOG(LogTemp, Warning, TEXT("X = %f Y = %f"), InputVector.X, InputVector.Y);
}
