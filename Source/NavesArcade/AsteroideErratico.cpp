// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroideErratico.h"

AAsteroideErratico::AAsteroideErratico()
{
	PrimaryActorTick.bCanEverTick = true;
	FrecuenciaOndulacion = 5.0f;
	AmplitudOndulacion = 300.0f;
	TiempoAcumulado = 0.0f;
}

void AAsteroideErratico::BeginPlay()
{
	// LLAMADA CRÍTICA: Resuelve el error "símbolo externo sin resolver"
	Super::BeginPlay();
}

void AAsteroideErratico::Tick(float DeltaTime)
{
	// Ejecuta el movimiento base hacia adelante de AsteroideDinamico
	Super::Tick(DeltaTime);

	TiempoAcumulado += DeltaTime;

	// Añadimos un movimiento perpendicular oscilatorio (Efecto zigzag)
	FVector Desviacion = FVector(0.0f, FMath::Sin(TiempoAcumulado * FrecuenciaOndulacion) * AmplitudOndulacion * DeltaTime, 0.0f);
	AddActorWorldOffset(Desviacion, true);
}