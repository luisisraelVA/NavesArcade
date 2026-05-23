// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelFab.h" // Interfaz de las fábricas de fase
#include "LevelBuilder.generated.h"

UCLASS()
class NAVESARCADE_API ALevelBuilder : public AActor
{
	GENERATED_BODY()

public:
	ALevelBuilder();

	// Asigna la fábrica (Fase 1 o Fase Final) que usará el Builder
	void SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica);

	// Cambia el tipo de asteroide que se va a spawnear dinámicamente
	void SetClaseAsteroide(TSubclassOf<class AAsteroideBase> NuevaClase);

	void SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide);
	void SpawnEnemigos(int32 Cantidad);
	void SpawnEnergia(int32 Cantidad);
	void SpawnBossFinal(FVector Ubicacion);

protected:
	TScriptInterface<ILevelFab> FabricaDeFase;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class AAsteroideBase> ClaseAsteroide;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class ANucleoEnergia> ClaseEnergia;
};
