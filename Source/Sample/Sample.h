// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"

#define debugf(Fmt, ...) UE_LOG(LogTemp, Warning, Fmt __VA_OPT__(,) __VA_ARGS__)