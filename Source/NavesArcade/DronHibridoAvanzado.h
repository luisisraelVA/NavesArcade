#pragma once

#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "DronHibridoAvanzado.generated.h"

UCLASS()
class NAVESARCADE_API ADronHibridoAvanzado : public AEnemigoBase
{
    GENERATED_BODY()

public:
    ADronHibridoAvanzado();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* Malla;

    UPROPERTY(EditAnywhere, Category = "AI")
    float VelocidadMovimiento = 400.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DistanciaDisparo = 1200.0f;

    UPROPERTY()
    class APawn* Objetivo;

    FTimerHandle TimerDisparo;

    UPROPERTY()
    class USoundBase* SonidoDisparoEnemigo;

    void DispararRafaga();
};