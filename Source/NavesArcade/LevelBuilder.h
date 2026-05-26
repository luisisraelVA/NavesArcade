#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelFab.h"
#include "LevelBuilder.generated.h"

UCLASS()
class NAVESARCADE_API ALevelBuilder : public AActor
{
    GENERATED_BODY()

public:
    ALevelBuilder();
    virtual void BeginPlay() override;

    void SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica);
    void SetClaseAsteroide(TSubclassOf<class AAsteroideBase> NuevaClase);
    void SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide);
    void GenerarFaseObjetivo();
    void NotificarMuerteEnemigo();

protected:
    UFUNCTION()
    void GeneracionContinua();

private:
    void SpawnNucleo();

    TScriptInterface<ILevelFab> FabricaDeFase;
    TSubclassOf<class AAsteroideBase> ClaseAsteroide;
    TSubclassOf<class ANucleoEnergia> ClaseEnergia;

    FTimerHandle TimerMundoAbierto;

    int32 DensidadAsteroides;
    float VelocidadGlobal;
    int32 FaseActualMision;
    int32 EnemigosVivosEnSector;
};