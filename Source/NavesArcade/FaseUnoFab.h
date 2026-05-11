// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelFab.h"
#include "FaseUnoFab.generated.h"

UCLASS()
class NAVESARCADE_API UFaseUnoFab : public UObject, public ILevelFab
{
	GENERATED_BODY()

public:
	// Implementación obligatoria de la interfaz
	virtual class AActor* CrearEnemigo(UWorld* World, FVector Location) override;

	// Retorna velocidad lenta para el aprendizaje del jugador [cite: 104]
	virtual float ObtenerVelocidadAmbiente() override { return 400.0f; }
};
