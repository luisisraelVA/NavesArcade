

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveNodriza.generated.h"

UCLASS()
class NAVESARCADE_API ANaveNodriza : public APawn
{
    GENERATED_BODY()

public:
    ANaveNodriza();

    virtual void Tick(float DeltaTime) override;

    void RecibirDano(float Dano);

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

    // --- NIVEL 9 ---
    float AnguloEspiralAcumulado;
    bool bEsNivelNueve;

    FTimerHandle TimerDisparoAbanico;
    FTimerHandle TimerInvocacion;
    FTimerHandle TimerHitFlash;
    FTimerHandle TimerMuerteCinematica;

    void DispararAbanico();

    // --- NUEVO ATAQUE HELICOIDAL ---
    void DispararEspiralHelicoidal();

    void InvocarKamikaze();

    void RestaurarMaterial();

    UFUNCTION()
    void FinalizarMuerteCinematica();
};