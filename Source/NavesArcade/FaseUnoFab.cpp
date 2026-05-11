// Fill out your copyright notice in the Description page of Project Settings.


#include "FaseUnoFab.h"
#include "EnemyFactory.h"

AActor* UFaseUnoFab::CrearEnemigo(UWorld* World, FVector Location)
{
	// La Fase 1 solo utiliza Drones Centinela (Enemigos fijos) [cite: 3, 110]
	return UEnemyFactory::SpawnEnemy(World, EEnemyType::Sentry, Location);
}
