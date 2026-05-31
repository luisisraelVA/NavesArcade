#pragma once

#include "CoreMinimal.h"
#include "AsteroideBase.h"
#include "AsteroideFractal.generated.h"

UCLASS()
class NAVESARCADE_API AAsteroideFractal : public AAsteroideBase
{
    GENERATED_BODY()

public:
    AAsteroideFractal();

protected:
    virtual void BeginPlay() override;
    virtual void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult) override;

    UPROPERTY(EditAnywhere, Category = "Fractal")
    int32 FragmentosHijos = 3;

    UPROPERTY(EditAnywhere, Category = "Fractal")
    TSubclassOf<AAsteroideBase> ClaseFragmento;

private:
    void Fragmentar();
};