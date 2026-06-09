#include "DueloCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Actores/DueloPelota.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ADueloCharacter::ADueloCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FuerzaDash = 3000.0f;
	bPuedeDashear = true;
	bEsInvulnerable = false; // Nace siendo vulnerable

	// Configuración de la colisión 
	BarreraColision = CreateDefaultSubobject<UBoxComponent>(TEXT("BarreraColision"));
	BarreraColision->SetupAttachment(RootComponent);
	BarreraColision->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));
	BarreraColision->SetBoxExtent(FVector(20.0f, 80.0f, 80.0f));
	BarreraColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BarreraColision->OnComponentBeginOverlap.AddDynamic(this, &ADueloCharacter::AlGolpearPelota);
}

void ADueloCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADueloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADueloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoverAction, ETriggerEvent::Triggered, this, &ADueloCharacter::Mover);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ADueloCharacter::EjecutarDash);
		EnhancedInputComponent->BindAction(GolpeAction, ETriggerEvent::Started, this, &ADueloCharacter::ActivarBarrera);
	}
}

void ADueloCharacter::Mover(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MovementVector.Y);
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementVector.X);
	}
}

void ADueloCharacter::EjecutarDash()
{
	if (!bPuedeDashear) return;

	FVector DireccionDash = GetLastMovementInputVector();

	if (DireccionDash.IsNearlyZero())
	{
		DireccionDash = GetActorForwardVector();
	}

	DireccionDash.Normalize();

	FVector Impulso = DireccionDash * FuerzaDash;
	Impulso.Z = 100.0f;
	LaunchCharacter(Impulso, true, true);

	Server_EjecutarDash(DireccionDash);

	bPuedeDashear = false;
	FTimerHandle TimerDash;
	GetWorldTimerManager().SetTimer(TimerDash, this, &ADueloCharacter::ResetearDash, 1.0f, false);
}

void ADueloCharacter::Server_EjecutarDash_Implementation(FVector Direccion)
{
	// INVULNERABILIDAD 
	bEsInvulnerable = true;

	FVector Impulso = Direccion * FuerzaDash;
	Impulso.Z = 100.0f;

	LaunchCharacter(Impulso, true, true);

	// timer en el servidor para apagar la invulnerabilidad 
	FTimerHandle TimerDashServer;
	GetWorldTimerManager().SetTimer(TimerDashServer, this, &ADueloCharacter::ResetearDash, 1.0f, false);
}

void ADueloCharacter::ResetearDash()
{
	bPuedeDashear = true;
	bEsInvulnerable = false;
}

void ADueloCharacter::ActivarBarrera()
{
	Server_ActivarBarrera();
}

void ADueloCharacter::Server_ActivarBarrera_Implementation()
{
	BarreraColision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FTimerHandle TimerBarrera;
	GetWorldTimerManager().SetTimer(TimerBarrera, this, &ADueloCharacter::DesactivarBarrera, 0.3f, false);
}

void ADueloCharacter::DesactivarBarrera()
{
	BarreraColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADueloCharacter::AlGolpearPelota(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA(ADueloPelota::StaticClass()))
	{
		ADueloPelota* Pelota = Cast<ADueloPelota>(OtherActor);
		if (Pelota)
		{
			FVector DireccionGolpe = Pelota->GetActorLocation() - GetActorLocation();
			Pelota->RecibirGolpe(DireccionGolpe, this);
		}
	}
}

//VICTORIA Y DERROTA
void ADueloCharacter::Client_TerminarPartida_Implementation(bool bEsGanador)
{
	// nodo rojo en el Blueprint
	BP_MostrarPantallaFinal(bEsGanador);
}