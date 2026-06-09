// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/DueloPlayerState.h" 
#include "Net/UnrealNetwork.h"

ADueloPlayerState::ADueloPlayerState()
{
	bReplicates = true;
	SetNetUpdateFrequency(10.0f);
}

void ADueloPlayerState::SetTeam(ETeam Team)
{
	TeamAsignado = Team;
}

ETeam ADueloPlayerState::GetTeam() const
{
	return TeamAsignado;
}

void ADueloPlayerState::OnRep_Team()
{
}

void ADueloPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADueloPlayerState, PuntosVitales);
	DOREPLIFETIME(ADueloPlayerState, TeamAsignado);
}