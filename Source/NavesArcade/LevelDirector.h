// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBuilder.h"
#include "LevelDirector.generated.h"

UCLASS()
class NAVESARCADE_API ALevelDirector : public AActor
{
	GENERATED_BODY()

public:
	ALevelDirector();

	void SetBuilder(ALevelBuilder* NuevoBuilder);

	void ConstruirNivel1();
	void ConstruirNivel2();
	void ConstruirNivel3();

	void ConstruirNivel4();
	void ConstruirNivel5();
	void ConstruirNivel6();

	void ConstruirNivel7();
	void ConstruirNivel8();
	void ConstruirNivel9();

	void ConstruirNivel10();
	void ConstruirNivel11();
	void ConstruirNivel12();

private:
	ALevelBuilder* Builder;
};
