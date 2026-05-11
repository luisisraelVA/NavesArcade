// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBuilder.h"
#include "AsteroideBase.h"
#include "NucleoEnergia.h"
#include "Kismet/KismetMathLibrary.h"

ALevelBuilder::ALevelBuilder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelBuilder::SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica)
{
	FabricaDeFase = NuevaFabrica;
}

void ALevelBuilder::SpawnAsteroides(int32 Cantidad)
{
	if (!FabricaDeFase || !ClaseAsteroide) return;

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(2000, 2000, 500));
		GetWorld()->SpawnActor<AAsteroideBase>(ClaseAsteroide, Loc, FRotator::ZeroRotator);

		// Aquí podrías usar FabricaDeFase->ObtenerVelocidadAmbiente() para ajustar los asteroides
	}
}

void ALevelBuilder::SpawnEnemigos(int32 Cantidad)
{
	if (!FabricaDeFase) return;

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(3000, 3000, 500));

		// Delegamos la creación del enemigo a la fábrica de la fase actual [cite: 3, 4]
		FabricaDeFase->CrearEnemigo(GetWorld(), Loc);
	}
}

void ALevelBuilder::SpawnEnergia(int32 Cantidad)
{
	if (!ClaseEnergia) return;

	for (int32 i = 0; i < Cantidad; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(2500, 2500, 500));
		GetWorld()->SpawnActor<ANucleoEnergia>(ClaseEnergia, Loc, FRotator::ZeroRotator);
	}
}

