
#include "NaveFacade.h"
#include "NaveJugador.h"
#include "InventoryComponent.h"
#include "WeaponSystem.h"
#include "AudioManager.h" // Incluimos el header para poder usar sus funciones

// NUEVOS INCLUDES PARA EL SPAWN EN C++ PURO
#include "Proyectil.h"            
#include "Engine/World.h"

UNaveFacade::UNaveFacade()
{
	PrimaryComponentTick.bCanEverTick = false;

	// INTEGRACIÓN DEL SUBCOMPONENTE:
	// Esto hace que el AudioManager aparezca automáticamente dentro de la Fachada en Unreal
	AudioManager = CreateDefaultSubobject<UAudioManager>(TEXT("AudioManagerComponent"));
}

void UNaveFacade::BeginPlay()
{
	Super::BeginPlay();

	NaveDuenia = Cast<ANaveJugador>(GetOwner());

	if (NaveDuenia)
	{
		Inventario = NaveDuenia->FindComponentByClass<UInventoryComponent>();
		SistemaArmas = NaveDuenia->FindComponentByClass<UWeaponSystem>();
	}
}

void UNaveFacade::ProcesarRecoleccionEnergia(float Cantidad)
{
	if (Inventario)
	{
		Inventario->AgregarEnergia(Cantidad);

		// Reproducir sonido de recolección/salto si tenemos energía suficiente
		if (AudioManager)
		{
			AudioManager->PlaySoundSalto();
		}
	}
}

void UNaveFacade::RecibirImpacto(float Dano)
{
	if (NaveDuenia)
	{
		NaveDuenia->RecibirDano(Dano);

		// Reproducir sonido de explosión/daño
		if (AudioManager)
		{
			AudioManager->PlaySoundExplosion();
		}
	}
}

// CORRECCIÓN DEFINITIVA EN EL FLUJO DE DISPARO:
void UNaveFacade::EjecutarDisparo()
{
	// Aseguramos la referencia a la nave dueña
	if (!NaveDuenia)
	{
		NaveDuenia = Cast<ANaveJugador>(GetOwner());
	}

	// 1. Verificamos que todo exista, incluyendo el Sistema de Armas
	if (NaveDuenia && SistemaArmas && GetWorld())
	{
		// 2. Calculamos la posición adelante (X + 150)
		FVector SpawnLoc = NaveDuenia->GetActorLocation() + (NaveDuenia->GetActorForwardVector() * 150.0f);
		FRotator SpawnRot = NaveDuenia->GetActorRotation();

		// 3. ¡LA CORRECCIÓN!: Usamos el Sistema de Armas para respetar la cadencia de tiro
		SistemaArmas->Disparar(SpawnLoc, SpawnRot);

		// 4. Reproducimos el sonido del disparo
		if (AudioManager)
		{
			AudioManager->PlaySoundDisparo();
		}
	}
}

bool UNaveFacade::PuedeSaltarDeNivel() const
{
	if (Inventario)
	{
		return Inventario->TieneEnergiaSuficiente();
	}
	return false;
}
