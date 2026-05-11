// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAVESARCADE_API UAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UAudioManager();

	// Funciones para reproducir sonidos específicos
	void PlaySoundDisparo();
	void PlaySoundExplosion();
	void PlaySoundSalto();

protected:
	// Punteros para los archivos de sonido (.wav / .mp3 importados)
	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoDisparo;

	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoExplosion;

	UPROPERTY(EditAnywhere, Category = "Sonidos")
	USoundBase* SonidoSalto;
};