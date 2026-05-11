// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyController::AEnemyController()
{
	// Constructor vacío o inicialización básica
}

void AEnemyController::BeginPlay()
{
	// Si no llamas a Super, el controlador no se registrará bien en el mundo
	Super::BeginPlay();

	if (ComportamientoEnemigo)
	{
		RunBehaviorTree(ComportamientoEnemigo);
	}
}