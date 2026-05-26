#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    EnergiaActual = 0.0f;
    EnergiaRequeridaNivel = 3;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInventoryComponent::AgregarEnergia(float Cantidad)
{
    EnergiaActual += Cantidad;
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Energía: %f"), EnergiaActual));
}

bool UInventoryComponent::TieneEnergiaSuficiente() const
{
    return EnergiaActual >= (float)EnergiaRequeridaNivel;
}

void UInventoryComponent::SetRequerimientoNivel(int32 NuevoRequerimiento)
{
    EnergiaRequeridaNivel = NuevoRequerimiento;
}