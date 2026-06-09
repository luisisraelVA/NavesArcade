#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelBuilder.h"
#include "LevelDirector.h"
#include "NavesArcadeGameMode.generated.h"

UCLASS(MinimalAPI)
class ANavesArcadeGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANavesArcadeGameMode();

    void AvanzarSiguienteNivel();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dificultad")
    EDificultad DificultadActual = EDificultad::Medio;

    int32 GetNivelActual() const { return NivelActual; }
    int32 GetNucleosRequeridos() const { return NucleosRequeridos; }
    void ActualizarRequerimientoPorNivel();

    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    bool bJuegoCompletado = false;

    UPROPERTY()
    TArray<AActor*> EnemigosActivos;

    // --- VARIABLE DEL MENÚ PRINCIPAL ---
    UPROPERTY(EditAnywhere, Category = "Feria | Interfaz")
    TSubclassOf<class UUserWidget> ClaseMenuWidget;

    void RegistrarEnemigo(AActor* NuevoEnemigo) { EnemigosActivos.AddUnique(NuevoEnemigo); }
    void DesregistrarEnemigo(AActor* EnemigoMuerto) { EnemigosActivos.Remove(EnemigoMuerto); }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ALevelBuilder* InstanciaBuilder;

    UPROPERTY()
    ALevelDirector* InstanciaDirector;

    int32 NivelActual;
    int32 NucleosRequeridos;

    void CargarRecetaNivel(int32 NumeroNivel);
    void FinalizarJuego();
    static constexpr int32 NUMERO_TOTAL_NIVELES = 12;
};