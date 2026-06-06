#pragma once

#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "NaveAcechadora.generated.h"

UCLASS()
class NAVESARCADE_API ANaveAcechadora : public AEnemigoBase
{
    GENERATED_BODY()

public:
    ANaveAcechadora();

protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;
    virtual void Tick(float DeltaTime) override;


    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    class USceneComponent* RaizMovimiento;

    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    class UStaticMeshComponent* MallaEnemigo;

    UPROPERTY(EditAnywhere, Category = "IA")
    float VelocidadPersecucion;

    FTimerHandle TimerDisparo;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float CadenciaDisparo;

    void Disparar();

    UPROPERTY()
    class APawn* TargetJugador;
};