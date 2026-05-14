// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.generated.h"

/**
 * 
 */
UCLASS()
class NAVESARCADE_API AAsteroideDinamico : public AAsteroideBase
{
	GENERATED_BODY()

public:
	AAsteroideDinamico();

	virtual void Tick(float DeltaTime) override;

	void ConfigurarMovimiento(float NuevaVelocidad, FVector NuevaDireccion);

protected:
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadMovimiento;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	FVector DireccionMovimiento;
};
