// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroideExplosivo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AAsteroideExplosivo::AAsteroideExplosivo()
{
	PrimaryActorTick.bCanEverTick = true;

	// C++ PURO: Cambiamos la forma a un Cono grande para el Nivel 3
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCono(TEXT("StaticMesh'/Game/MisNaves/meteor_detailed.meteor_detailed'"));
	if (MallaCono.Succeeded() && MallaAsteroide)
	{
		MallaAsteroide->SetStaticMesh(MallaCono.Object);
		MallaAsteroide->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f)); // Tamaño peligroso
	}

	// Atributos de dificultad alta para el sector 3
	DanoPorChoque = 50.0f;
	VelocidadMovimiento = 700.0f;
}

void AAsteroideExplosivo::BeginPlay()
{
	Super::BeginPlay();
}
