// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelBuilder.h"
#include "LevelDirector.h"
#include "FaseUnoFab.h"
#include "FaseFinalFab.h"
#include "NavesArcadeGameMode.generated.h"

UCLASS(MinimalAPI)
class ANavesArcadeGameMode : public AGameModeBase
{
    GENERATED_BODY()


public:
    ANavesArcadeGameMode();

    void AvanzarSiguienteNivel();

    // Dificultad global (Fácil, Medio, Difícil)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dificultad")
    EDificultad DificultadActual = EDificultad::Medio;



    // Nivel actual (1..6)
    int32 GetNivelActual() const { return NivelActual; }
    int32 GetNucleosRequeridos() const { return NucleosRequeridos; }
    void ActualizarRequerimientoPorNivel();

    // Variable para saber si el juego ha terminado (victoria)
    bool bJuegoCompletado = false;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ALevelBuilder* InstanciaBuilder;

    UPROPERTY()
    ALevelDirector* InstanciaDirector;

    UPROPERTY()
    UFaseUnoFab* FabricaFaseUno;

    UPROPERTY()
    UFaseFinalFab* FabricaFaseFinal;

    UPROPERTY()
    class UFaseAvanzadaFab* FabricaFaseAvanzada;

    int32 NivelActual;
    int32 NucleosRequeridos;

    void CargarRecetaNivel(int32 NumeroNivel);
    void LimpiarMapa();

private:
    static constexpr int32 NUMERO_TOTAL_NIVELES = 12;

public:
    // Lista optimizada de enemigos activos para el HUD
    UPROPERTY()
    TArray<AActor*> EnemigosActivos;

    void RegistrarEnemigo(AActor* NuevoEnemigo) { EnemigosActivos.AddUnique(NuevoEnemigo); }
    void DesregistrarEnemigo(AActor* EnemigoMuerto) { EnemigosActivos.Remove(EnemigoMuerto); }
};