#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DodgerHUD.generated.h"

UCLASS()
class NAVESARCADE_API ADodgerHUD : public AHUD
{
    GENERATED_BODY()

public:
    ADodgerHUD();

    // Esta es la función nativa que se ejecuta cada fotograma para dibujar en pantalla
    virtual void DrawHUD() override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UFont* FuentePrincipal;

    TArray<AActor*> NucleosCacheados;
    float TiempoUltimoCacheo = 0.0f;

    int32 NivelActualGuardado = 1;
};