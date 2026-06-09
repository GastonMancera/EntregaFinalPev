// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h" 
#include "DueloGameMode.generated.h"

UCLASS()
class TPFINALPEV_API ADueloGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADueloGameMode();

	virtual void BeginPlay() override;
	virtual void StartMatch() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// --- FUNCIÓN DEL ÁRBITRO ---
	void VerificarGanador(class ADueloCharacter* Atacante, class ADueloCharacter* Victima);

protected:
	FTimerHandle MatchStartTimer;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float MatchStartTimeDuration;
};