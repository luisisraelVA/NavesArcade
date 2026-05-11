// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaveFacade.generated.h"

// Forward Declarations para mejorar tiempos de compilación
class ANaveJugador;
class UInventoryComponent;
class UWeaponSystem;
class UAudioManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVESARCADE_API UNaveFacade : public UActorComponent
{
	GENERATED_BODY()

public:
	// Constructor
	UNaveFacade();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ANaveJugador* NaveDuenia;

	UPROPERTY()
	UInventoryComponent* Inventario;

	UPROPERTY()
	UWeaponSystem* SistemaArmas;

	// --- SUBCOMPONENTE DE AUDIO ---
	// VisibleAnywhere permite que veamos sus propiedades (los slots de sonido) en el editor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioManager* AudioManager;

public:
	// --- MÉTODOS DE LA FACHADA ---

	/** Procesa la recolección de energía e invoca sonido de recolección */
	void ProcesarRecoleccionEnergia(float Cantidad);

	/** Maneja el daño recibido e invoca sonido de impacto */
	void RecibirImpacto(float Dano);

	/** Ejecuta el disparo e invoca sonido de arma */
	void EjecutarDisparo();

	/** Consulta si se cumplen los requisitos de energía */
	bool PuedeSaltarDeNivel() const;
};