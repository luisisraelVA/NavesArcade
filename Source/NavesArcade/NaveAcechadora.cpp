#include "NaveAcechadora.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyController.h" 
#include "NaveJugador.h" 

ANaveAcechadora::ANaveAcechadora()
{
	PrimaryActorTick.bCanEverTick = true;

	MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
	RootComponent = MallaEnemigo;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshAsset.Succeeded())
	{
		MallaEnemigo->SetStaticMesh(MeshAsset.Object);
	}

	VelocidadPersecucion = 650.0f; // Más rápidas para ser un desafío real

	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANaveAcechadora::BeginPlay()
{
	Super::BeginPlay();
	TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ANaveAcechadora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CORRECCIÓN DE IA: Si el jugador reapareció o no se encontró al inicio, lo volvemos a buscar
	if (!TargetJugador)
	{
		TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	if (TargetJugador)
	{
		// Rotación hacia el jugador
		FVector DireccionHaciaJugador = (TargetJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator RotacionDestino = DireccionHaciaJugador.Rotation();
		FRotator RotacionSuave = FMath::RInterpTo(GetActorRotation(), RotacionDestino, DeltaTime, 4.0f);
		SetActorRotation(RotacionSuave);

		// Avanzar
		AddActorWorldOffset(GetActorForwardVector() * VelocidadPersecucion * DeltaTime, true);

		// Efecto Levitación
		float EfectoHover = FMath::Sin(GetWorld()->GetTimeSeconds() * 5.0f) * 150.0f * DeltaTime;
		AddActorWorldOffset(FVector(0.0f, 0.0f, EfectoHover), true);

		// Ataque Kamikaze
		float Distancia = FVector::Dist(GetActorLocation(), TargetJugador->GetActorLocation());
		if (Distancia < 350.0f) // Distancia justa para no fallar el choque
		{
			ANaveJugador* Jugador = Cast<ANaveJugador>(TargetJugador);
			if (Jugador)
			{
				Jugador->RecibirDano(25.0f);
				Destroy(); // Se sacrifica para hacerte daño
			}
		}
	}
}