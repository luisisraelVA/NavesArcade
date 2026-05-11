// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Proyectil.generated.h"

UCLASS()
class NAVESARCADE_API AProyectil : public AActor
{
	GENERATED_BODY()

	public:
		AProyectil();

	protected:
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, Category = "Componentes")
		class USphereComponent* Colision;

		UPROPERTY(VisibleAnywhere, Category = "Componentes")
		class UStaticMeshComponent* Malla;

		UPROPERTY(VisibleAnywhere, Category = "Movimiento")
		class UProjectileMovementComponent* Movimiento;

	public:
		virtual void Tick(float DeltaTime) override;
	};