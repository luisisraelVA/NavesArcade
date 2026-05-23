// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsteroideDinamico.h"
#include "AsteroideErratico.generated.h"

UCLASS()
class NAVESARCADE_API AAsteroideErratico : public AAsteroideDinamico
{
	GENERATED_BODY()

public:
	AAsteroideErratico();

	virtual void Tick(float DeltaTime) override;

protected:
	// Aseguramos la declaración de BeginPlay
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movimiento Erratico")
	float FrecuenciaOndulacion;

	UPROPERTY(EditAnywhere, Category = "Movimiento Erratico")
	float AmplitudOndulacion;

	float TiempoAcumulado;
};