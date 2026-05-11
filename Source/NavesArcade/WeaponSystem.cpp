// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponSystem.h"
#include "Proyectil.h"
#include "Engine/World.h"
#include "TimerManager.h"

UWeaponSystem::UWeaponSystem()
{
	PrimaryComponentTick.bCanEverTick = false; // Eficiencia: No necesita ejecutarse cada frame

	CadenciaDisparo = 0.5f; // Medio segundo entre disparos por defecto
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

		// POO: Delegamos la creación del proyectil al motor usando la clase configurada
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
