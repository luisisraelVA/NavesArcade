// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponSystem.h"
#include "Proyectil.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h" // Cabecera necesaria para el Cast del Instigator

UWeaponSystem::UWeaponSystem()
{
	PrimaryComponentTick.bCanEverTick = false; // Eficiencia: No necesita ejecutarse cada frame
	CadenciaDisparo = 0.3f; // Tiempo de enfriamiento entre ráfagas
	bPuedeDisparar = true;
}

void UWeaponSystem::BeginPlay()
{
	Super::BeginPlay();

	// C++ Puro: Si no se asigna en editor, forzamos la clase estática nativa
	if (!ClaseProyectil)
	{
		ClaseProyectil = AProyectil::StaticClass();
	}
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
