// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DueloPelota.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class ADueloCharacter;

UCLASS()
class TPFINALPEV_API ADueloPelota : public AActor
{
	GENERATED_BODY()

public:
	ADueloPelota();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* ColisionPelota;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* MallaPelota;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movimiento")
	UProjectileMovementComponent* MovimientoProyectil;

	void RecibirGolpe(FVector NuevaDireccion, ADueloCharacter* NuevoDuenio);

	//LÓGICA DE DAÑO 
	UFUNCTION()
	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// detectar cuando la pelota rebota contra una pared
	UFUNCTION()
	void AlRebotar(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	// servidor le avisa a los clientes que reproduzcan el efecto
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_EfectoImpactoJugador();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_EfectoImpactoPared();

	// Eventos del Camera Shake
	UFUNCTION(BlueprintImplementableEvent, Category = "Efectos")
	void BP_EfectoImpactoJugador();

	UFUNCTION(BlueprintImplementableEvent, Category = "Efectos")
	void BP_EfectoImpactoPared();
};