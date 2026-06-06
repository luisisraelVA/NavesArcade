#pragma once
#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "DronCentinela.generated.h"

UCLASS()
class NAVESARCADE_API ADronCentinela : public AEnemigoBase
{
    GENERATED_BODY()
public:
    ADronCentinela();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaDron;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA")
    float VelocidadPatrulla;

    UPROPERTY()
    AActor* ObjetivoActual;

    FTimerHandle TimerCicloDisparo;
    void EjecutarDisparoLaser();
};