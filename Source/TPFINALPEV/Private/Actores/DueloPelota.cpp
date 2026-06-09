// Fill out your copyright notice in the Description page of Project Settings.

#include "Actores/DueloPelota.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "DueloCharacter.h" 
#include "Framework/GameMode/DueloGameState.h" 
#include "Framework/GameMode/DueloPlayerState.h" 
#include "Framework/GameMode/DueloGameMode.h" 

ADueloPelota::ADueloPelota()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);

	// Colisión
	ColisionPelota = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionPelota"));
	RootComponent = ColisionPelota;
	ColisionPelota->InitSphereRadius(40.0f);

	ColisionPelota->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ColisionPelota->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ColisionPelota->OnComponentBeginOverlap.AddDynamic(this, &ADueloPelota::AlSuperponerse);

	// Visual
	MallaPelota = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPelota"));
	MallaPelota->SetupAttachment(RootComponent);
	MallaPelota->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Movimiento y Físicas
	MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
	MovimientoProyectil->UpdatedComponent = ColisionPelota;

	MovimientoProyectil->InitialSpeed = 1000.0f;
	MovimientoProyectil->MaxSpeed = 20000.0f;
	MovimientoProyectil->bRotationFollowsVelocity = true;
	MovimientoProyectil->bShouldBounce = true;
	MovimientoProyectil->Bounciness = 1.0f;
	MovimientoProyectil->Friction = 0.0f;
	MovimientoProyectil->ProjectileGravityScale = 0.0f;

	MovimientoProyectil->bConstrainToPlane = true;
	MovimientoProyectil->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));

	//  rebotes contra las paredes para el Camera Shake
	MovimientoProyectil->OnProjectileBounce.AddDynamic(this, &ADueloPelota::AlRebotar);
}

void ADueloPelota::BeginPlay()
{
	Super::BeginPlay();
}

void ADueloPelota::RecibirGolpe(FVector NuevaDireccion, ADueloCharacter* NuevoDuenio)
{
	if (HasAuthority())
	{
		SetOwner(NuevoDuenio);

		NuevaDireccion.Z = 0.0f;
		NuevaDireccion.Normalize();

		float VelocidadActual = MovimientoProyectil->Velocity.Size();
		// Arranca en 1500 pero suma un 15% de velocidad con cada golpe 
		float NuevaVelocidad = FMath::Max(1500.0f, VelocidadActual * 1.15f);

		MovimientoProyectil->Velocity = NuevaDireccion * NuevaVelocidad;
	}
}

void ADueloPelota::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor)
	{
		ADueloCharacter* JugadorImpactado = Cast<ADueloCharacter>(OtherActor);
		if (JugadorImpactado)
		{
			// Validar contra la cápsula del personaje
			if (OtherComp == JugadorImpactado->GetCapsuleComponent())
			{
				//  si el jugador hizo un Dash es invulnerable
				if (JugadorImpactado->bEsInvulnerable)
				{
					return;
				}

				if (GetOwner() != JugadorImpactado && GetOwner() != nullptr)
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("¡PUNTOS ROBADOS!"));
					}

					ADueloCharacter* Atacante = Cast<ADueloCharacter>(GetOwner());
					if (Atacante)
					{
						ADueloPlayerState* PS_Victima = JugadorImpactado->GetPlayerState<ADueloPlayerState>();
						ADueloPlayerState* PS_Atacante = Atacante->GetPlayerState<ADueloPlayerState>();
						ADueloGameState* GS = GetWorld()->GetGameState<ADueloGameState>();

						if (PS_Victima && PS_Atacante && GS)
						{
							float VelocidadActual = MovimientoProyectil->Velocity.Size();

							// El daño representa el 15% de la velocidad de la pelota
							int32 DanioCalculado = FMath::RoundToInt(VelocidadActual * 0.15f);
							int32 PuntosRobados = FMath::Min(DanioCalculado, PS_Victima->PuntosVitales);

							PS_Victima->PuntosVitales -= PuntosRobados;
							PS_Atacante->PuntosVitales += PuntosRobados;

							//  Camera Shake a todos los jugadores
							Multicast_EfectoImpactoJugador();

							if (PS_Victima->GetTeam() == ETeam::CanchaA)
							{
								GS->PorcentajeCanchaA = (float)PS_Victima->PuntosVitales / 2500.0f;
							}
							else if (PS_Atacante->GetTeam() == ETeam::CanchaA)
							{
								GS->PorcentajeCanchaA = (float)PS_Atacante->PuntosVitales / 2500.0f;
							}

							
							ADueloGameMode* GM = GetWorld()->GetAuthGameMode<ADueloGameMode>();
							if (GM)
							{
								GM->VerificarGanador(Atacante, JugadorImpactado);
							}
						}
					}
				}
			}
		}
	}
}


// CS (GAME FEEL)


void ADueloPelota::AlRebotar(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (HasAuthority())
	{
		// Si la pelota rebota contra una pared a más de 2000 de velocidad
		if (ImpactVelocity.Size() > 2000.0f)
		{
			// avisamos a  los clientes que reproduzcan la vibración 
			Multicast_EfectoImpactoPared();
		}
	} 
}

void ADueloPelota::Multicast_EfectoImpactoJugador_Implementation()
{
	BP_EfectoImpactoJugador(); // evento rojo en  Blueprint
}

void ADueloPelota::Multicast_EfectoImpactoPared_Implementation()
{
	BP_EfectoImpactoPared(); // evento rojo en  Blueprint
}