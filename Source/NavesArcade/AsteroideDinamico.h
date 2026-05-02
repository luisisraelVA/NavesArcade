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

protected:
	
	float VelocidadMovimiento;
	FVector DireccionMovimiento;

public:
	virtual void Tick(float DeltaTime) override;
	
};
