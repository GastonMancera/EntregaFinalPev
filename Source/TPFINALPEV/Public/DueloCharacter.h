// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DueloCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UBoxComponent;

UCLASS()
class TPFINALPEV_API ADueloCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADueloCharacter();

	// VARIABLE PARA LA INVULNERABILIDAD
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combate")
	bool bEsInvulnerable;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// VARIABLES DE ENHANCED INPUT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoverAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GolpeAction;

	//FUNCIÓN DE MOVIMIENTO
	void Mover(const FInputActionValue& Value);

	// HABILIDADES
	void EjecutarDash();

	// se ejecutará en el servidor para validar el dash
	UFUNCTION(Server, Reliable)
	void Server_EjecutarDash(FVector Direccion);

	void ResetearDash();
	bool bPuedeDashear;

	UPROPERTY(EditAnywhere, Category = "Habilidades")
	float FuerzaDash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combate")
	UBoxComponent* BarreraColision;

	void ActivarBarrera();

	UFUNCTION(Server, Reliable)
	void Server_ActivarBarrera();

	void DesactivarBarrera();

	UFUNCTION()
	void AlGolpearPelota(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	
	UFUNCTION(Client, Reliable)
	void Client_TerminarPartida(bool bEsGanador);

	// Evento para Blueprint donde mostraremos el Widget final
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void BP_MostrarPantallaFinal(bool bEsGanador);
};