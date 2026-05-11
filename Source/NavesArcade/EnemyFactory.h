// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyFactory.generated.h"

// Definimos los tipos de enemigos disponibles según el diseño del juego [cite: 3, 58]
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Sentry      UMETA(DisplayName = "Dron Centinela"),
	Hunter      UMETA(DisplayName = "Nave Acechadora"),
	Boss        UMETA(DisplayName = "Nave Nodriza")
};

UCLASS()
class NAVESARCADE_API UEnemyFactory : public UObject
{
	GENERATED_BODY()

public:
	/** * Método Factory: Centraliza la instanciación de las amenazas del juego [cite: 2, 3]
	 * @param World Contexto del nivel actual
	 * @param Type Tipo de enemigo a fabricar (Sentry, Hunter o Boss)
	 * @param Location Posición de aparición en el espacio 3D
	 */
	static class AActor* SpawnEnemy(UWorld* World, EEnemyType Type, FVector Location);
};
