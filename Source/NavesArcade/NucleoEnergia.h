// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NucleoEnergia.generated.h"

UCLASS()
class NAVESARCADE_API ANucleoEnergia : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANucleoEnergia();

protected:

	virtual void BeginPlay() override;

	class USphereComponent* EsferaColision;

	class UStaticMeshComponent* MallaNucleo;

	float ValorEnergia;

	
	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	
	virtual void Tick(float DeltaTime) override;

};
