// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalSalto.generated.h"

UCLASS()
class NAVESARCADE_API APortalSalto : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortalSalto();

protected:
		virtual void BeginPlay() override;
	class USphereComponent* EsferaColision;

	class UStaticMeshComponent* MallaPortal;

	
	float EnergiaRequerida;

	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

	virtual void Tick(float DeltaTime) override;

};
