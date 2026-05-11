
#include "NaveFacade.h"
#include "NaveJugador.h"
#include "InventoryComponent.h"
#include "WeaponSystem.h"
#include "AudioManager.h" // Incluimos el header para poder usar sus funciones

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

void UNaveFacade::EjecutarDisparo()
{
	if (SistemaArmas && NaveDuenia)
	{
		FVector SpawnLoc = NaveDuenia->GetActorLocation() + (NaveDuenia->GetActorForwardVector() * 100.0f);
		FRotator SpawnRot = NaveDuenia->GetActorRotation();

		SistemaArmas->Disparar(SpawnLoc, SpawnRot);

		// Reproducir sonido de disparo
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