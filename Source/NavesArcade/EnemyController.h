// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

UCLASS()
class NAVESARCADE_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

protected:
	// El uso de override es fundamental para el sistema virtual
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "IA")
	class UBehaviorTree* ComportamientoEnemigo;
};
