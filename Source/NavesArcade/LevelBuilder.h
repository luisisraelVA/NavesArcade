#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// Declaraciones adelantadas para no incluir innecesariamente archivos pesados
class AAsteroideBase;
class ANucleoEnergia;

#include "LevelBuilder.generated.h"

// Definición de delegados
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNucleoRecolectadoSignature);
DECLARE_DELEGATE_OneParam(FOnGenerarFaseSignature, int32 /*FaseActual*/);

UCLASS()
class NAVESARCADE_API ALevelBuilder : public AActor
{
    GENERATED_BODY()

public:
    ALevelBuilder();
    virtual void BeginPlay() override;
    void SetNucleoPendiente(bool bEstado);
    // Métodos de control
    void ResetEnemigos();
    void SetClaseAsteroide(TSubclassOf<AAsteroideBase> NuevaClase);
    void SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide);

    // Lógica de juego
    void GenerarFaseObjetivo();
    void NotificarMuerteEnemigo();
    void RegistrarEnemigos(int32 Cantidad);

    // Getters y Setters seguros
    void SetJefeAparecido(bool bEstado) { bJefeAparecido = bEstado; }
    bool GetJefeAparecido() const { return bJefeAparecido; }

    // Delegados
    UPROPERTY(BlueprintAssignable, Category = "Eventos Avanzados")
    FOnNucleoRecolectadoSignature OnNucleoRecolectado;

    FOnGenerarFaseSignature OnGenerarFase;

protected:
    UFUNCTION()
    void GeneracionContinua();

    // Variable para controlar el núcleo (hecha protegida para seguridad)
    bool bNucleoPendiente;

    // Esto lo necesitamos público para que el LevelDirector pueda resetearlo
    // Pero solo a través de ResetEnemigos()
    int32 EnemigosVivosEnSector;

private:
    void SpawnNucleo();

    // Clases
    TSubclassOf<AAsteroideBase> ClaseAsteroide;
    TSubclassOf<ANucleoEnergia> ClaseEnergia;

    FTimerHandle TimerMundoAbierto;

    int32 DensidadAsteroides;
    float VelocidadGlobal;
    int32 FaseActualMision;

    bool bJefeAparecido;
    bool bJefeDerrotado;
};