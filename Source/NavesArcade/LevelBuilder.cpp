// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilder.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "NucleoEnergia.h"
#include "EnemyFactory.h"
#include "Kismet/KismetMathLibrary.h"
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

void ALevelBuilder::SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide)
{
	if (!ClaseAsteroide || !GetWorld())
	{
		return;
	}

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(
			GetActorLocation(),
			FVector(3000.0f, 3000.0f, 700.0f)
		);

		AAsteroideBase* Asteroide = GetWorld()->SpawnActor<AAsteroideBase>(
			ClaseAsteroide,
			Loc,
			FRotator::ZeroRotator
		);

		AAsteroideDinamico* AsteroideDinamico = Cast<AAsteroideDinamico>(Asteroide);

		if (AsteroideDinamico)
		{
			FVector DireccionAleatoria = FVector(
				FMath::RandRange(-1.0f, 1.0f),
				FMath::RandRange(-1.0f, 1.0f),
				FMath::RandRange(-0.2f, 0.2f)
			);

			AsteroideDinamico->ConfigurarMovimiento(
				VelocidadAsteroide,
				DireccionAleatoria
			);
		}
	}
}

void ALevelBuilder::SpawnEnemigos(int32 Cantidad)
{
	if (!FabricaDeFase || !GetWorld())
	{
		return;
	}

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(
			GetActorLocation(),
			FVector(3500.0f, 3500.0f, 800.0f)
		);

		FabricaDeFase->CrearEnemigo(GetWorld(), Loc);
	}
}

void ALevelBuilder::SpawnEnergia(int32 Cantidad)
{
	if (!ClaseEnergia || !GetWorld())
	{
		return;
	}

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
	if (!GetWorld())
	{
		return;
	}

	UEnemyFactory::SpawnEnemy(
		GetWorld(),
		EEnemyType::Boss,
		Ubicacion
	);
}

