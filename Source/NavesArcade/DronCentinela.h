// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DronCentinela.generated.h"

UCLASS()
class NAVESARCADE_API ADronCentinela : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADronCentinela();

protected:
	virtual void BeginPlay() override;
	
	class UStaticMeshComponent* MallaDron;

	
	class USphereComponent* EsferaDeteccion;

	
	
	TArray<FVector> PuntosDeRuta;

	
	float VelocidadPatrulla;

	int32 IndicePuntoActual;

	
	void AlDetectarJugador(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

};
