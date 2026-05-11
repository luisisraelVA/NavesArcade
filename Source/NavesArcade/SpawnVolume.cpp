// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
// Los headers de motor siempre después del header de la clase
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyFactory.h" 

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaSpawn"));
	// Establecemos la raíz del componente
	RootComponent = AreaSpawn;

	// Extensión del área para el volumen de aparición [cite: 24]
	AreaSpawn->SetBoxExtent(FVector(1000.0f, 1000.0f, 1000.0f));
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	// Se ejecuta al inicio de cada sector [cite: 156]
	EjecutarSpawn();
}

void ASpawnVolume::EjecutarSpawn()
{
	if (GetWorld())
	{
		FVector Origen = GetActorLocation();
		FVector Extension = AreaSpawn->GetScaledBoxExtent();

		// Cálculo de punto aleatorio en el espacio 3D [cite: 43, 81]
		FVector PuntoAleatorio = UKismetMathLibrary::RandomPointInBoundingBox(Origen, Extension);

		// Invocación al Factory Method para crear la entidad [cite: 3, 24]
		UEnemyFactory::SpawnEnemy(GetWorld(), TipoDeEnemigo, PuntoAleatorio);
	}
}

void ASpawnVolume::SpawnEnemigo()
{
	EjecutarSpawn();
}
