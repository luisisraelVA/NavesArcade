// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveAcechadora.generated.h"

UCLASS()
class NAVESARCADE_API ANaveAcechadora : public APawn
{
	GENERATED_BODY()

public:
	ANaveAcechadora();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UStaticMeshComponent* MallaEnemigo;

	UPROPERTY(EditAnywhere, Category = "IA")
	float VelocidadPersecucion;

	// Referencia para la lógica de persecución activa [cite: 58, 87]
	class APawn* TargetJugador;

public:
	virtual void Tick(float DeltaTime) override;
};