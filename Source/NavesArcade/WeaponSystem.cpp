#include "WeaponSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h" 
#include "NaveJugador.h"
#include "AsteroideBase.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h"

UWeaponSystem::UWeaponSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	CadenciaDisparo = 0.3f; // Tiempo de enfriamiento entre ráfagas
	bPuedeDisparar = true;
}

void UWeaponSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponSystem::Disparar(FVector Ubicacion, FRotator Rotacion)
{
	if (!bPuedeDisparar || !ClaseProyectil) return;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		// CORRECCIÓN: Esta línea fuerza la aparición de la bala aunque roce la nave
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Delegamos la creación del proyectil al motor
		World->SpawnActor<AProyectil>(ClaseProyectil, Ubicacion, Rotacion, SpawnParams);

		// Sistema de enfriamiento (Cooldown)
		bPuedeDisparar = false;
		World->GetTimerManager().SetTimer(TimerHandle_Cadencia, this, &UWeaponSystem::ResetearDisparo, CadenciaDisparo, false);
	}
}

void UWeaponSystem::ResetearDisparo()
{
	bPuedeDisparar = true;
}