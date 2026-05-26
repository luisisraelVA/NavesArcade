#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Proyectil.h"
#include "WeaponSystem.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVESARCADE_API UWeaponSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponSystem();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Armamento")
    TSubclassOf<class AProyectil> ClaseProyectil;

    UPROPERTY(EditAnywhere, Category = "Armamento")
    float CadenciaDisparo;

    bool bPuedeDisparar;
    FTimerHandle TimerHandle_Cadencia;
    float UltimoTiempoDisparo;

    UFUNCTION()
    void ResetearDisparo();

public:
    void Disparar(FVector Ubicacion, FRotator Rotacion);
    void SetCadenciaDisparo(float NuevaCadencia) { CadenciaDisparo = NuevaCadencia; }
};