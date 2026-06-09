// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamType.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
    None UMETA(DisplayName = "Ninguno"),
    CanchaA UMETA(DisplayName = "Cancha A"),
    CanchaB UMETA(DisplayName = "Cancha B")
};