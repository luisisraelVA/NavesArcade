// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBuilder.h"
#include "LevelDirector.generated.h"

UCLASS()
class NAVESARCADE_API ALevelDirector : public AActor
{
	GENERATED_BODY()

public:
	ALevelDirector();

	// Asigna el constructor que el director va a manejar
	void SetBuilder(ALevelBuilder* NuevoBuilder);

	// Receta para un nivel de Introducción (Fase 1: Niveles 1-3)
	void ConstruirNivelIntroduccion();

	// Receta para un nivel de Densidad Máxima (Fase 4: Niveles 10-12)
	void ConstruirNivelFinal();

private:
	// Referencia al constructor actual 
	ALevelBuilder* Builder;
};
