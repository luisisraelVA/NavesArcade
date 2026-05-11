// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h"
#include "Engine/World.h"

AActor* UEnemyFactory::SpawnEnemy(UWorld* World, EEnemyType Type, FVector Location)
{
	if (!World)
	{
		return nullptr;
	}

	// Configuración de spawn para evitar bloqueos por colisión inicial [cite: 24]
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// El Factory decide qué clase instanciar basándose en el Enum definido [cite: 3]
	switch (Type)
	{
	case EEnemyType::Sentry:
		// Spawnea el Dron Centinela (Enemigo fijo/patrulla) [cite: 83]
		return World->SpawnActor<ADronCentinela>(ADronCentinela::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

	case EEnemyType::Hunter:
		// Spawnea la Nave Acechadora (IA de persecución activa) [cite: 17, 86]
		return World->SpawnActor<ANaveAcechadora>(ANaveAcechadora::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

	case EEnemyType::Boss:
		// Spawnea la Nave Nodriza (Jefe final del Sector 12) [cite: 18, 152]
		return World->SpawnActor<ANaveNodriza>(ANaveNodriza::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

	default:
		return nullptr;
	}
}
