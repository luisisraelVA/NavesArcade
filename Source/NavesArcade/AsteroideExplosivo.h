#pragma once

#include "CoreMinimal.h"
#include "AsteroideDinamico.h"
#include "AsteroideExplosivo.generated.h"

UCLASS()
class NAVESARCADE_API AAsteroideExplosivo : public AAsteroideDinamico
{
    GENERATED_BODY()

public:
    AAsteroideExplosivo();

protected:
    virtual void BeginPlay() override;
};