// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Framework/Utils/TeamType.h" 
#include "DueloPlayerState.generated.h"

UCLASS()
class TPFINALPEV_API ADueloPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADueloPlayerState();

	void SetTeam(ETeam Team);
	ETeam GetTeam() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Stats Duelo")
	int32 PuntosVitales;

	UPROPERTY(ReplicatedUsing = OnRep_Team, BlueprintReadWrite, Category = "Stats Duelo")
	ETeam TeamAsignado = ETeam::None;

	UFUNCTION()
	void OnRep_Team();
};