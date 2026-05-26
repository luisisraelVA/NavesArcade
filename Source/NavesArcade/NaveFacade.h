#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.h"
#include "NaveJugador.h"
#include "NaveFacade.generated.h"

class ANaveJugador;
class UInventoryComponent;
class UWeaponSystem;
class UAudioManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVESARCADE_API UNaveFacade : public UActorComponent
{
    GENERATED_BODY()

public:
    UNaveFacade();

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    ANaveJugador* NaveDuenia;

    UPROPERTY()
    UInventoryComponent* Inventario;

    UPROPERTY()
    UWeaponSystem* SistemaArmas;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioManager* AudioManager;

    float UltimoTiempoDisparo;

public:
    void ProcesarRecoleccionEnergia(float Cantidad);
    void RecibirImpacto(float Dano);
    void EjecutarDisparo();
    bool PuedeSaltarDeNivel() const;

    FORCEINLINE float ObtenerVidaNave() const { return NaveDuenia ? NaveDuenia->GetIntegridadEstructural() : 0.0f; }
    FORCEINLINE float ObtenerEnergiaNave() const { return Inventario ? Inventario->GetEnergiaActual() : 0.0f; }
};