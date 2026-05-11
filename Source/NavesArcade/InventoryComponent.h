// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAVESARCADE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
		UInventoryComponent();

	protected:
		virtual void BeginPlay() override;

		// Variable protegida que almacena la energía (POO: Encapsulamiento)
		UPROPERTY(EditAnywhere, Category = "Recursos")
		float EnergiaActual;

		UPROPERTY(EditAnywhere, Category = "Recursos")
		float EnergiaRequeridaNivel;

	public:
		// Método llamado por el NucleoEnergia para sumar recursos
		void AgregarEnergia(float Cantidad);

		// Función de consulta (POO: Abstracción)
		UFUNCTION(BlueprintPure, Category = "Inventario")
		bool TieneEnergiaSuficiente() const;

		// Getter para obtener la energía actual
		float GetEnergiaActual() const { return EnergiaActual; }
	};