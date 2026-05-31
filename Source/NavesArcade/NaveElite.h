#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaveElite.generated.h"

UCLASS()
class NAVESARCADE_API ANaveElite : public AActor
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

    UPROPERTY(EditAnywhere, Category = "Combate")
    float Escudo;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float Salud;

    UPROPERTY()
    class APawn* Jugador;

    FTimerHandle TimerDisparo;

    void Disparar();

public:
    void RecibirDano(float Cantidad);
};