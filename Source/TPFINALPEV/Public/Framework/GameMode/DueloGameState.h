// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DueloGameState.generated.h"

UCLASS()
class TPFINALPEV_API ADueloGameState : public AGameState
{
	GENERATED_BODY()

public:
	ADueloGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(ReplicatedUsing = OnRep_PorcentajeCancha, BlueprintReadWrite, Category = "Match")
	float PorcentajeCanchaA;

	UFUNCTION()
	void OnRep_PorcentajeCancha();
};
