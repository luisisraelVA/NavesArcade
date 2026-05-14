// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioManager.generated.h"

class USoundBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVESARCADE_API UAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UAudioManager();

	void PlaySoundDisparo();
	void PlaySoundExplosion();
	void PlaySoundSalto();
	void PlaySoundBoss();

protected:
	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoDisparo;

	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoExplosion;

	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoSalto;

	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoBoss;
};