#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LevelFab.generated.h"

UINTERFACE(MinimalAPI)
class ULevelFab : public UInterface
{
    GENERATED_BODY()
};

class NAVESARCADE_API ILevelFab
{
    GENERATED_BODY()

public:
    virtual class AActor* CrearEnemigo(class UWorld* World, FVector Location) = 0;
    virtual float ObtenerVelocidadAmbiente() = 0;
};