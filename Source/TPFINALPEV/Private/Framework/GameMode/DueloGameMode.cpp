// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/DueloGameMode.h"
#include "Framework/GameMode/DueloGameState.h"
#include "Framework/GameMode/DueloPlayerState.h"
#include "DueloCharacter.h" 

ADueloGameMode::ADueloGameMode()
{
	GameStateClass = ADueloGameState::StaticClass();
	PlayerStateClass = ADueloPlayerState::StaticClass();

	bDelayedStart = true;
	MatchStartTimeDuration = 3.0f;
}

void ADueloGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MatchStartTimer, this, &ADueloGameMode::StartMatch, MatchStartTimeDuration, false);
}

void ADueloGameMode::StartMatch()
{
	Super::StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("¡FIGHT!"));
	}
}

void ADueloGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ADueloPlayerState* PS = Cast<ADueloPlayerState>(NewPlayer->PlayerState);
	if (PS)
	{
		PS->PuntosVitales = 1250;
		ETeam Team = (GetNumPlayers() % 2 == 0) ? ETeam::CanchaB : ETeam::CanchaA;
		PS->SetTeam(Team);
	}
}


void ADueloGameMode::VerificarGanador(ADueloCharacter* Atacante, ADueloCharacter* Victima)
{
	if (!Atacante || !Victima) return;

	ADueloPlayerState* PS_Atacante = Atacante->GetPlayerState<ADueloPlayerState>();

	if (PS_Atacante && PS_Atacante->PuntosVitales >= 2500)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("¡K.O.! PARTIDA FINALIZADA POR EL GAMEMODE."));
		}

		// Avisamos a cada pantalla de forma privada
		Atacante->Client_TerminarPartida(true);
		Victima->Client_TerminarPartida(false);

		// Destruimos al perdedor
		Victima->SetLifeSpan(0.2f);
	}
}