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
    virtual void DrawHUD() override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UFont* FuentePrincipal;
};