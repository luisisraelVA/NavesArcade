#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveAcechadora.generated.h"

UCLASS()
class NAVESARCADE_API ANaveAcechadora : public APawn
{
    GENERATED_BODY()

public:
    ANaveAcechadora();

protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;
   

    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    class UStaticMeshComponent* MallaEnemigo;

    UPROPERTY(EditAnywhere, Category = "IA")
    float VelocidadPersecucion;

    // Nuevo: Temporizador para disparar
    FTimerHandle TimerDisparo;

    // Nuevo: Cadencia de disparo (segundos entre disparos)
    UPROPERTY(EditAnywhere, Category = "Combate")
    float CadenciaDisparo;

    void Disparar();

    UPROPERTY()
    class APawn* TargetJugador;

public:
    virtual void Tick(float DeltaTime) override;
};