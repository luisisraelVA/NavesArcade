// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelDirector.h"

ALevelDirector::ALevelDirector()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelDirector::SetBuilder(ALevelBuilder* NuevoBuilder)
{
	Builder = NuevoBuilder;
}

void ALevelDirector::ConstruirNivelIntroduccion()
{
	if (!Builder) return;

	// Receta para niveles 1-3: Pocos asteroides y enemigos para alfabetización mecánica [cite: 36, 97]
	Builder->SpawnAsteroides(10);
	Builder->SpawnEnergia(5);
	Builder->SpawnEnemigos(2); // Usualmente Drones Centinela en esta fase [cite: 110]
}

void ALevelDirector::ConstruirNivelFinal()
{
	if (!Builder) return;

	// Receta para niveles 10-12: Alta densidad de amenazas y naves de élite [cite: 141, 148]
	Builder->SpawnAsteroides(50);
	Builder->SpawnEnemigos(15); // Naves Acechadoras y Drones suicidas [cite: 144, 148]
	Builder->SpawnEnergia(20);
}
