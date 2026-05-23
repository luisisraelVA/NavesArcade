// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	// Desactivamos el tick porque el inventario solo reacciona a eventos (recolección)
	PrimaryComponentTick.bCanEverTick = false;

	EnergiaActual = 0.0f;
	EnergiaRequeridaNivel = 60.0f; // Meta definida en el diseño del juego
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Implementación de la función que recibe los datos del NucleoEnergia
void UInventoryComponent::AgregarEnergia(float Cantidad)
{
	EnergiaActual += Cantidad;

	// Feedback visual en consola para verificar la recolección
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("Energía Recibida: %f | Total en Inventario: %f"), Cantidad, EnergiaActual));
	}
}

bool UInventoryComponent::TieneEnergiaSuficiente() const
{
	return EnergiaActual >= EnergiaRequeridaNivel;
}
