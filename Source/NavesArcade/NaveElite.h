#pragma once
#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "NaveElite.generated.h"

UCLASS()
class NAVESARCADE_API ANaveElite : public AEnemigoBase
{
    GENERATED_BODY()
public:
    ANaveElite();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* Malla;

    FTimerHandle TimerDisparo;
    void Disparar();

    UPROPERTY()
    class APawn* Jugador;
};