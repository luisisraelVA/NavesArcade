// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelFab.h"
#include "FaseAvanzadaFab.generated.h"

UCLASS()
class NAVESARCADE_API UFaseAvanzadaFab : public UObject, public ILevelFab
{
    GENERATED_BODY()

public:
    // Implementación de los métodos de la interfaz ILevelFab
    virtual class AActor* CrearEnemigo(class UWorld* World, FVector Location) override;
    virtual float ObtenerVelocidadAmbiente() override { return 1800.0f; } // Velocidad de entorno más desafiante
};