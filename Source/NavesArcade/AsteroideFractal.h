// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsteroideDinamico.h"
#include "AsteroideFractal.generated.h"

UCLASS()
class NAVESARCADE_API AAsteroideFractal : public AAsteroideDinamico
{
    GENERATED_BODY()

public:
    AAsteroideFractal();

protected:
    virtual void BeginPlay() override;

    // Evita división infinita
    UPROPERTY(EditAnywhere, Category = "Fractal")
    bool bEsFragmento;

    // Escala de los hijos
    UPROPERTY(EditAnywhere, Category = "Fractal")
    float EscalaHijos;

public:

    UFUNCTION()
    void AlRecibirImpactoProyectil(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};