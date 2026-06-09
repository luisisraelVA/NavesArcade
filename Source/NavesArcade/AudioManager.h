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
    void PlaySoundDisparo2();
    void PlaySoundExplosion();
    void PlaySoundSalto();
    void PlaySoundBoss();
    void PlaySoundChoqueEnemigo();

    // Funciones de la nueva capa de sonido
    void PlaySoundImpacto();
    void PlaySoundMusicaFondo();
    void PlaySoundRecogerNucleo();
    void PlaySoundDerrota();
    void PlaySoundMenosVida();

protected:
    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoDisparo;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoDisparo2;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoExplosion;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoSalto;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoBoss;

    // Nuevas variables referenciadas
    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoImpacto;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* MusicaFondo;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoNucleo;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoDerrota;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoMenosVida;

    UPROPERTY(EditAnywhere, Category = "Sonidos")
    USoundBase* SonidoChoqueEnemigo; 
};