#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DronSuicida.generated.h"

UCLASS()
class NAVESARCADE_API ADronSuicida : public AActor
{
    GENERATED_BODY()

public:
    ADronSuicida();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* Malla;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class USphereComponent* ColisionDano;  // Para daño por contacto (opcional)

    UPROPERTY(EditAnywhere, Category = "Combate")
    float CadenciaDisparo;   // Segundos entre disparos

    UPROPERTY()
    class APawn* Jugador;

    FTimerHandle TimerDisparo;

    void Disparar();

public:
    void RecibirDano(float Cantidad);  // Para que muera con disparos
};