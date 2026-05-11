// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavesArcadeGameMode.h"
#include "NaveJugador.h"
#include "DodgerHUD.h"
#include "EnemyFactory.h" // Incluye la fábrica para instanciar enemigos 

ANavesArcadeGameMode::ANavesArcadeGameMode()
{
	// Asigna la nave del jugador por defecto
	DefaultPawnClass = ANaveJugador::StaticClass();

	// Asigna el HUD para la interfaz
	HUDClass = ADodgerHUD::StaticClass();
}

void ANavesArcadeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Ejemplo de uso de la Factory para spawnear el Jefe Final en el Sector 12
	// FVector UbicacionBoss = FVector(5000.0f, 0.0f, 0.0f);
	// UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Boss, UbicacionBoss);
}