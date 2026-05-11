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

	// Componentes necesarios para la lógica del .cpp
	UPROPERTY(VisibleAnywhere, Category = "Colision")
	class USphereComponent* EsferaColision;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UStaticMeshComponent* MallaProyectil;

	UPROPERTY(VisibleAnywhere, Category = "Movimiento")
	class UProjectileMovementComponent* MovimientoProyectil;

	// Función de impacto
	UFUNCTION()
	void AlImpactar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void Tick(float DeltaTime) override;
};