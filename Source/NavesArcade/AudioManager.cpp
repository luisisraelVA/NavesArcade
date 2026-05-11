// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"
#include "Kismet/GameplayStatics.h"

UAudioManager::UAudioManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAudioManager::PlaySoundDisparo()
{
	if (SonidoDisparo)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SonidoDisparo, GetOwner()->GetActorLocation());
	}
}

void UAudioManager::PlaySoundExplosion()
{
	if (SonidoExplosion)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SonidoExplosion, GetOwner()->GetActorLocation());
	}
}

void UAudioManager::PlaySoundSalto()
{
	if (SonidoSalto)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SonidoSalto, GetOwner()->GetActorLocation());
	}
}