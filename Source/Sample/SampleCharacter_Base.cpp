// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleCharacter_Base.h"

// Sets default values
ASampleCharacter_Base::ASampleCharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASampleCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASampleCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASampleCharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

