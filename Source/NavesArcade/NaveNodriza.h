#pragma once

#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "NaveNodriza.generated.h"

UCLASS()
class NAVESARCADE_API ANaveNodriza : public AEnemigoBase
{
    GENERATED_BODY()

public:
    ANaveNodriza();
    virtual void Tick(float DeltaTime) override;
    virtual void RecibirDano(float Dano) override;

    float GetVidaActual() const { return VidaActual; }
    float GetVidaMaxima() const { return VidaMaxima; }

protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    class UStaticMeshComponent* MallaJefe;

    UPROPERTY()
    class UParticleSystem* EfectoMuerteMasiva;

    float VidaActual;
    float VidaMaxima;
    bool bFaseDos;
    bool bMuerto;
    float AnguloEspiralAcumulado;
    bool bEsNivelNueve;

    FTimerHandle TimerDisparoAbanico;
    FTimerHandle TimerInvocacion;
    FTimerHandle TimerHitFlash;
    FTimerHandle TimerMuerteCinematica;

    void DispararAbanico();
    void DispararEspiralHelicoidal();
    void InvocarKamikaze();
    void RestaurarMaterial();

    UFUNCTION()
    void FinalizarMuerteCinematica();
};