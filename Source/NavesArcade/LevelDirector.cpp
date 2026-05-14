// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelDirector.h"

ALevelDirector::ALevelDirector()
{
	PrimaryActorTick.bCanEverTick = false;
	Builder = nullptr;
}

void ALevelDirector::SetBuilder(ALevelBuilder* NuevoBuilder)
{
	Builder = NuevoBuilder;
}

// NIVELES 1, 2, 3

void ALevelDirector::ConstruirNivel1()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(5, 300.0f);
	Builder->SpawnEnergia(8);
	Builder->SpawnEnemigos(1);
}

void ALevelDirector::ConstruirNivel2()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(8, 400.0f);
	Builder->SpawnEnergia(7);
	Builder->SpawnEnemigos(2);
}

void ALevelDirector::ConstruirNivel3()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(12, 500.0f);
	Builder->SpawnEnergia(6);
	Builder->SpawnEnemigos(3);
}

//NIVELES 4, 5, 6

void ALevelDirector::ConstruirNivel4()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(16, 600.0f);
	Builder->SpawnEnergia(6);
	Builder->SpawnEnemigos(4);
}

void ALevelDirector::ConstruirNivel5()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(20, 700.0f);
	Builder->SpawnEnergia(5);
	Builder->SpawnEnemigos(5);
}

void ALevelDirector::ConstruirNivel6()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(24, 800.0f);
	Builder->SpawnEnergia(5);
	Builder->SpawnEnemigos(6);
}

// NIVELES 7, 8, 9

void ALevelDirector::ConstruirNivel7()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(30, 900.0f);
	Builder->SpawnEnergia(4);
	Builder->SpawnEnemigos(8);
}

void ALevelDirector::ConstruirNivel8()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(36, 1050.0f);
	Builder->SpawnEnergia(4);
	Builder->SpawnEnemigos(10);
}

void ALevelDirector::ConstruirNivel9()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(42, 1200.0f);
	Builder->SpawnEnergia(3);
	Builder->SpawnEnemigos(12);
}
//NIVELES 10, 11, 12

void ALevelDirector::ConstruirNivel10()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(50, 1350.0f);
	Builder->SpawnEnergia(4);
	Builder->SpawnEnemigos(14);
}

void ALevelDirector::ConstruirNivel11()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(60, 1500.0f);
	Builder->SpawnEnergia(3);
	Builder->SpawnEnemigos(18);
}

void ALevelDirector::ConstruirNivel12()
{
	if (!Builder) return;

	Builder->SpawnAsteroides(70, 1700.0f);
	Builder->SpawnEnergia(2);
	Builder->SpawnEnemigos(20);

	Builder->SpawnBossFinal(FVector(5000.0f, 0.0f, 500.0f));
}
