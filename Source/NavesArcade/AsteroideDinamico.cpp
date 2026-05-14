// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroideDinamico.h"

AAsteroideDinamico::AAsteroideDinamico()
{
	PrimaryActorTick.bCanEverTick = true;

	VelocidadMovimiento = 500.0f;
	DireccionMovimiento = FVector(-1.0f, 0.0f, 0.0f);
}

void AAsteroideDinamico::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Movimiento = DireccionMovimiento.GetSafeNormal() * VelocidadMovimiento * DeltaTime;

	AddActorWorldOffset(Movimiento, true);
}

void AAsteroideDinamico::ConfigurarMovimiento(float NuevaVelocidad, FVector NuevaDireccion)
{
	VelocidadMovimiento = NuevaVelocidad;
	DireccionMovimiento = NuevaDireccion.GetSafeNormal();
}