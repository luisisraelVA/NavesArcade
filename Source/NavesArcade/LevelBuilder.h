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

	// Métodos de la "receta" de construcción 
	void SpawnAsteroides(int32 Cantidad);
	void SpawnEnemigos(int32 Cantidad);
	void SpawnEnergia(int32 Cantidad);

protected:
	// Referencia a la fábrica actual para obtener enemigos y velocidades [cite: 3, 4]
	TScriptInterface<ILevelFab> FabricaDeFase;

	// Clases base para el spawn (Configurables en el editor o vía código)
	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class AAsteroideBase> ClaseAsteroide;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class ANucleoEnergia> ClaseEnergia;
};