// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LevelFab.generated.h"

// Esta clase no se toca, es para que Unreal reconozca la interfaz en el editor 
UINTERFACE(MinimalAPI)
class ULevelFab : public UInterface
{
	GENERATED_BODY()
};

// Esta es la interfaz real que usarán tus fábricas de fase 
class NAVESARCADE_API ILevelFab
{
	GENERATED_BODY()

public:
	// Virtual puro (= 0) obliga a FaseUno y FaseFinal a implementarlo 
	virtual class AActor* CrearEnemigo(UWorld* World, FVector Location) = 0;

	virtual float ObtenerVelocidadAmbiente() = 0;
};
