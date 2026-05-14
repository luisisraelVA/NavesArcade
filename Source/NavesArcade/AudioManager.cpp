// Fill out your copyright notice in the Description page of Project Settings.
#include "AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UAudioManager::UAudioManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	SonidoDisparo = nullptr;
	SonidoExplosion = nullptr;
	SonidoSalto = nullptr;
	SonidoBoss = nullptr;
}

void UAudioManager::PlaySoundDisparo()
{
	if (SonidoDisparo && GetOwner())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SonidoDisparo,
			GetOwner()->GetActorLocation()
		);
	}
}

void UAudioManager::PlaySoundExplosion()
{
	if (SonidoExplosion && GetOwner())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SonidoExplosion,
			GetOwner()->GetActorLocation()
		);
	}
}

void UAudioManager::PlaySoundSalto()
{
	if (SonidoSalto && GetOwner())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SonidoSalto,
			GetOwner()->GetActorLocation()
		);
	}
}

void UAudioManager::PlaySoundBoss()
{
	if (SonidoBoss && GetOwner())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SonidoBoss,
			GetOwner()->GetActorLocation()
		);
	}
}