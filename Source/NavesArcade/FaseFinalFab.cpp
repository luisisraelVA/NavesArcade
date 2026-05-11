// Fill out your copyright notice in the Description page of Project Settings.


#include "FaseFinalFab.h"
#include "EnemyFactory.h"

AActor* UFaseFinalFab::CrearEnemigo(UWorld* World, FVector Location)
{
	// La Fase Final utiliza Naves Acechadoras con persecución activa [cite: 17, 148]
	return UEnemyFactory::SpawnEnemy(World, EEnemyType::Hunter, Location);
}
