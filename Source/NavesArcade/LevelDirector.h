#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelDirector.generated.h"



UENUM(BlueprintType)
enum class EDificultad : uint8 {
    Facil, Medio, Dificil
};

class ALevelBuilder;

UCLASS()
class NAVESARCADE_API ALevelDirector : public AActor
{
    GENERATED_BODY()

public:
    ALevelDirector();
    void SetBuilder(ALevelBuilder* NuevoBuilder);
    void SetDificultad(EDificultad NuevaDificultad);

    // Funciones de construcción
    void ConstruirNivel1();
    void ConstruirNivel2();
    void ConstruirNivel3();
    void ConstruirNivel4();
    void ConstruirNivel5();
    void ConstruirNivel6();
    void ConstruirNivel7();
    void ConstruirNivel8();
    void ConstruirNivel9();
    void ConstruirNivel10();
    void ConstruirNivel11();
    void ConstruirNivel12();

protected:
    UFUNCTION()
    void ManejarGeneracionFase(int32 FaseActual);

    UPROPERTY()
    ALevelBuilder* Builder;

    EDificultad Dificultad;

private:
    void ConfigurarDificultad(float& Velocidad, int32& Cantidad);
};