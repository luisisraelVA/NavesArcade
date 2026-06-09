#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBuilder.h"
#include "LevelDirector.generated.h"


UENUM(BlueprintType)
enum class EDificultad : uint8
{
    Facil      UMETA(DisplayName = "Fácil"),
    Medio      UMETA(DisplayName = "Medio"),
    Dificil    UMETA(DisplayName = "Difícil")
};

UCLASS()
class NAVESARCADE_API ALevelDirector : public AActor
{
    GENERATED_BODY()

public:
    ALevelDirector();

    void SetBuilder(ALevelBuilder* NuevoBuilder);
    void SetDificultad(EDificultad NuevaDificultad);

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

private:
    void ManejarGeneracionFase(int32 FaseActual);

    ALevelBuilder* Builder;
    EDificultad Dificultad;
};