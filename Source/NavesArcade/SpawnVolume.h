// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// Importante: El include de la Factory debe estar ANTES de .generated.h
#include "EnemyFactory.h" 
#include "SpawnVolume.generated.h"

UCLASS()
class NAVESARCADE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ASpawnVolume();

protected:
	// Cambiamos el orden de los métodos para cumplir con el estándar
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	class UBoxComponent* AreaSpawn;

	// Tipo de enemigo configurable para los 12 niveles [cite: 3, 23, 24]
	UPROPERTY(EditAnywhere, Category = "Spawning")
	EEnemyType TipoDeEnemigo;

	void EjecutarSpawn();

public:
	void SpawnEnemigo();
};