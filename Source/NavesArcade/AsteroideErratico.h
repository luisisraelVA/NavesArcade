#pragma once

#include "CoreMinimal.h"
#include "AsteroideDinamico.h"
#include "AsteroideErratico.generated.h"

UCLASS()
class NAVESARCADE_API AAsteroideErratico : public AAsteroideDinamico
{
    GENERATED_BODY()

public:
    AAsteroideErratico();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Movimiento Erratico")
    float FrecuenciaOndulacion;

    UPROPERTY(EditAnywhere, Category = "Movimiento Erratico")
    float AmplitudOndulacion;

    float TiempoAcumulado;
};