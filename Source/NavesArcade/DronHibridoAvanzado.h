// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h" // <-- CAMBIADO: Inclusión obligatoria para APawn
#include "DronHibridoAvanzado.generated.h"

UCLASS()
class NAVESARCADE_API ADronHibridoAvanzado : public APawn // <-- CAMBIADO: Ahora hereda de APawn
{
    GENERATED_BODY()

public:
    ADronHibridoAvanzado();

protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaDron;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float VelocidadActual;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float VelocidadFrenetica;

    UPROPERTY()
    AActor* ObjetivoJugador;

    FTimerHandle TimerCicloDisparo;
    FVector DireccionPatrulla;

    void EjecutarDisparoLaser();

    // --- PATRÓN OBSERVER: El método callback que reaccionará al evento ---
    UFUNCTION()
    void EscucharAlertaNucleo();

public:
    virtual void Tick(float DeltaTime) override;
};