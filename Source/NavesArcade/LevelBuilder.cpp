// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilder.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "NucleoEnergia.h"
#include "PortalSalto.h"
#include "EnemyFactory.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h" // Cabecera nativa para detectar al jugador
#include "Engine/World.h"

ALevelBuilder::ALevelBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	ClaseAsteroide = AAsteroideDinamico::StaticClass();
	ClaseEnergia = ANucleoEnergia::StaticClass();
}

void ALevelBuilder::SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica)
{
	FabricaDeFase = NuevaFabrica;
}

void ALevelBuilder::SetClaseAsteroide(TSubclassOf<AAsteroideBase> NuevaClase)
{
	ClaseAsteroide = NuevaClase;
}

void ALevelBuilder::SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide)
{
	if (!ClaseAsteroide || !GetWorld()) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(
			GetActorLocation(),
			FVector(3000.0f, 3000.0f, 700.0f)
		);

		AActor* NuevoActor = GetWorld()->SpawnActor<AActor>(ClaseAsteroide, Loc, FRotator::ZeroRotator);

		AAsteroideDinamico* AsteroideDinamico = Cast<AAsteroideDinamico>(NuevoActor);
		if (AsteroideDinamico)
		{
			FVector DireccionHaciaNave = FVector(-1.0f, 0.0f, 0.0f);
			if (PlayerPawn)
			{
				DireccionHaciaNave = (PlayerPawn->GetActorLocation() - Loc).GetSafeNormal();
			}
			AsteroideDinamico->ConfigurarMovimiento(VelocidadAsteroide, DireccionHaciaNave);
		}
	}

	// UBICACIÓN CORREGIDA: Spawnea el portal usando la ubicación actual del Builder 
	// para que nazca dentro de tu zona de juego visible
	FVector PosicionPortal = GetActorLocation() + FVector(2000.0f, 0.0f, 200.0f);
	GetWorld()->SpawnActor<APortalSalto>(PosicionPortal, FRotator::ZeroRotator);
}

void ALevelBuilder::SpawnEnemigos(int32 Cantidad)
{
	if (!FabricaDeFase.GetObject() || !GetWorld()) return;

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(
			GetActorLocation(),
			FVector(3500.0f, 3500.0f, 800.0f)
		);

		ILevelFab* InterfazNativa = Cast<ILevelFab>(FabricaDeFase.GetObject());

		if (InterfazNativa)
		{
			InterfazNativa->CrearEnemigo(GetWorld(), Loc);
		}
	}
}

void ALevelBuilder::SpawnEnergia(int32 Cantidad)
{
	if (!ClaseEnergia || !GetWorld()) return;

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(
			GetActorLocation(),
			FVector(2500.0f, 2500.0f, 600.0f)
		);

		GetWorld()->SpawnActor<ANucleoEnergia>(
			ClaseEnergia,
			Loc,
			FRotator::ZeroRotator
		);
	}
}

void ALevelBuilder::SpawnBossFinal(FVector Ubicacion)
{
	if (!GetWorld()) return;

	UEnemyFactory::SpawnEnemy(
		GetWorld(),
		EEnemyType::Boss,
		Ubicacion
	);
}