#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemigoBase.h" 
#include "LevelBuilder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNucleoRecolectadoSignature);


DECLARE_DELEGATE_OneParam(FOnGenerarFaseSignature, int32 /*FaseActual*/);

UCLASS()
class NAVESARCADE_API ALevelBuilder : public AActor
{
    GENERATED_BODY()

public:
    ALevelBuilder();
    virtual void BeginPlay() override;

    void SetClaseAsteroide(TSubclassOf<class AAsteroideBase> NuevaClase);
    void SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide);

    void GenerarFaseObjetivo();
    void NotificarMuerteEnemigo();
    void RegistrarEnemigos(int32 Cantidad);

    bool bNucleoPendiente;

    UPROPERTY(BlueprintAssignable, Category = "Eventos Avanzados")
    FOnNucleoRecolectadoSignature OnNucleoRecolectado;

    FOnGenerarFaseSignature OnGenerarFase;

    void SetJefeAparecido(bool bEstado) { bJefeAparecido = bEstado; }
    bool GetJefeAparecido() const { return bJefeAparecido; }

protected:
    UFUNCTION()
    void GeneracionContinua();

private:
    void SpawnNucleo();

    TSubclassOf<class AAsteroideBase> ClaseAsteroide;
    TSubclassOf<class ANucleoEnergia> ClaseEnergia;

    FTimerHandle TimerMundoAbierto;

    int32 DensidadAsteroides;
    float VelocidadGlobal;
    int32 FaseActualMision;
    int32 EnemigosVivosEnSector;

    bool bJefeAparecido;
    bool bJefeDerrotado;
};