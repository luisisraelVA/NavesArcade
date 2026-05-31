#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelFab.h"
#include "FaseAvanzadaFab.generated.h"

UCLASS()
class NAVESARCADE_API UFaseAvanzadaFab : public UObject, public ILevelFab
{
    GENERATED_BODY()

public:
    virtual class AActor* CrearEnemigo(class UWorld* World, FVector Location) override;
    virtual float ObtenerVelocidadAmbiente() override { return 2000.0f; }
};