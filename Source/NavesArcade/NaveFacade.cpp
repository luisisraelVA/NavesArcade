
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
	// Si por el orden de inicialización de Unreal NaveDuenia aún está vacío, lo asignamos aquí de golpe
	if (!NaveDuenia)
	{
		NaveDuenia = Cast<ANaveJugador>(GetOwner());
	}

	// 1. Verificamos que la Fachada conozca a la nave dueña y que el mundo exista
	if (NaveDuenia && GetWorld())
	{
		// 2. Calculamos la posición adelante (X + 150) para evitar que la colisión del proyectil choque con la nave al nacer
		FVector SpawnLoc = NaveDuenia->GetActorLocation() + (NaveDuenia->GetActorForwardVector() * 150.0f);
		FRotator SpawnRot = NaveDuenia->GetActorRotation();

		FActorSpawnParameters ParametrosSpawn;
		ParametrosSpawn.Owner = NaveDuenia;
		ParametrosSpawn.Instigator = NaveDuenia->GetInstigator();

		// 3. Spawneamos el proyectil directamente en el mundo usando código puro en tiempo de ejecución
		GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), SpawnLoc, SpawnRot, ParametrosSpawn);

		// 4. Reproducimos el sonido del disparo que programó tu grupo
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
