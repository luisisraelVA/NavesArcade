// Fill out your copyright notice in the Description page of Project Settings.
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

public:
	void ProcesarRecoleccionEnergia(float Cantidad);
	void RecibirImpacto(float Dano);
	void EjecutarDisparo();
	bool PuedeSaltarDeNivel() const;

	// CONEXIÓN AL HUD: Getters para actualizar la UI en tiempo real
	FORCEINLINE float ObtenerVidaNave() const { return NaveDuenia ? NaveDuenia->GetIntegridadEstructural() : 0.0f; }
	FORCEINLINE float ObtenerEnergiaNave() const { return Inventario ? Inventario->GetEnergiaActual() : 0.0f; }
};
