// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/DueloGameState.h"
#include "Net/UnrealNetwork.h"

ADueloGameState::ADueloGameState()
{
	bReplicates = true;
	SetNetUpdateFrequency(10.0f);
	PorcentajeCanchaA = 0.5f;
}

void ADueloGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADueloGameState, PorcentajeCanchaA);
}

void ADueloGameState::OnRep_PorcentajeCancha()
{
}