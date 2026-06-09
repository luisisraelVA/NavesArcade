#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVESARCADE_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    void AgregarEnergia(float Cantidad);
    bool TieneEnergiaSuficiente() const;
    float GetEnergiaActual() const { return EnergiaActual; }

    void SetRequerimientoNivel(int32 NuevoRequerimiento);
    int32 GetRequerimientoNivel() const { return EnergiaRequeridaNivel; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Recursos")
    float EnergiaActual;

    UPROPERTY(EditAnywhere, Category = "Recursos")
    int32 EnergiaRequeridaNivel;  
};