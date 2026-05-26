#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelFab.h"
#include "FaseUnoFab.generated.h"

UCLASS()
class NAVESARCADE_API UFaseUnoFab : public UObject, public ILevelFab
{
    GENERATED_BODY()

public:
    virtual class AActor* CrearEnemigo(class UWorld* World, FVector Location) override;
    virtual float ObtenerVelocidadAmbiente() override { return 400.0f; }
};