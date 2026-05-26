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
        float PitchAleatorio = FMath::RandRange(0.85f, 1.15f);
        UGameplayStatics::PlaySoundAtLocation(this, SonidoDisparo, GetOwner()->GetActorLocation(), 1.0f, PitchAleatorio);
    }
}

void UAudioManager::PlaySoundExplosion()
{
    if (SonidoExplosion && GetOwner())
    {
        float PitchAleatorio = FMath::RandRange(0.7f, 1.1f);
        UGameplayStatics::PlaySoundAtLocation(this, SonidoExplosion, GetOwner()->GetActorLocation(), 1.2f, PitchAleatorio);
    }
}

void UAudioManager::PlaySoundSalto()
{
    if (SonidoSalto && GetOwner())
    {
        UGameplayStatics::PlaySoundAtLocation(this, SonidoSalto, GetOwner()->GetActorLocation());
    }
}

void UAudioManager::PlaySoundBoss()
{
    if (SonidoBoss && GetOwner())
    {
        UGameplayStatics::PlaySoundAtLocation(this, SonidoBoss, GetOwner()->GetActorLocation());
    }
}