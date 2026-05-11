// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelFab.h"
#include "FaseFinalFab.generated.h"

UCLASS()
class NAVESARCADE_API UFaseFinalFab : public UObject, public ILevelFab
{
	GENERATED_BODY()

public:
	// Implementación obligatoria de la interfaz
	virtual class AActor* CrearEnemigo(UWorld* World, FVector Location) override;

	// Retorna velocidad máxima para alta intensidad [cite: 146]
	virtual float ObtenerVelocidadAmbiente() override { return 1500.0f; }
};
