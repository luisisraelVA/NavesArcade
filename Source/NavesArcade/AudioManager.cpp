#include "AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h" 
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

UAudioManager::UAudioManager()
{
    PrimaryComponentTick.bCanEverTick = false;


    MusicaComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicaComponent"));
    MusicaComponent->bAutoActivate = false; 
    // DISPAROS
    static ConstructorHelpers::FObjectFinder<USoundBase> AudioDisparoObj(TEXT("SoundWave'/Game/Sonidos/SonidoNaveJugador.SonidoNaveJugador'"));
    if (AudioDisparoObj.Succeeded()) SonidoDisparo = AudioDisparoObj.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> AudioDisparoObj2(TEXT("SoundWave'/Game/Sonidos/DisparoEnemigos.DisparoEnemigos'"));
    if (AudioDisparoObj2.Succeeded()) SonidoDisparo2 = AudioDisparoObj2.Object;

    // IMPACTO
    static ConstructorHelpers::FObjectFinder<USoundBase> AudioImpactoObj(TEXT("SoundWave'/Game/Sonidos/impacto.impacto'"));
    if (AudioImpactoObj.Succeeded()) SonidoImpacto = AudioImpactoObj.Object;

    // NUCLEO
    static ConstructorHelpers::FObjectFinder<USoundBase> AudioNucleoObj(TEXT("SoundWave'/Game/Sonidos/nucleopickup.nucleopickup'"));
    if (AudioNucleoObj.Succeeded()) SonidoNucleo = AudioNucleoObj.Object;

    //// MUSICA DE FONDO
    //static ConstructorHelpers::FObjectFinder<USoundBase> AudioMusicaObj(TEXT("SoundWave'/Game/Sonidos/SonidoFondo.SonidoFondo'"));
    //if (AudioMusicaObj.Succeeded())
    //{
    //    MusicaFondo = AudioMusicaObj.Object;

    //    // --- TRUCO PRO: Forzamos el bucle infinito directamente por código ---
    //    if (USoundWave* WaveMusica = Cast<USoundWave>(MusicaFondo))
    //    {
    //        WaveMusica->bLooping = true; // Esto hace que se repita para siempre
    //    }

    //}

    MusicaFondo = nullptr;

    if (MusicaComponent)
    {
        MusicaComponent->SetSound(nullptr); // Vaciamos el componente a la fuerza
        MusicaComponent->bAutoActivate = false;
    }

    // GAME OVER Y MENOS VIDA
    static ConstructorHelpers::FObjectFinder<USoundBase> AudioDerrotaObj(TEXT("SoundWave'/Game/Sonidos/GameOver.GameOver'"));
    if (AudioDerrotaObj.Succeeded()) SonidoDerrota = AudioDerrotaObj.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> AudioMenosVidaObj(TEXT("SoundWave'/Game/Sonidos/menosvida.menosvida'"));
    if (AudioMenosVidaObj.Succeeded()) SonidoMenosVida = AudioMenosVidaObj.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> AudioChoqueEnemigoObj(TEXT("SoundWave'/Game/Sonidos/ChoqueEnemigos.ChoqueEnemigos'"));
    if (AudioChoqueEnemigoObj.Succeeded())
    {
        SonidoChoqueEnemigo = AudioChoqueEnemigoObj.Object;
    }

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

void UAudioManager::PlaySoundDisparo2()
{
    if (SonidoDisparo2 && GetOwner())
    {
        float PitchAleatorio = FMath::RandRange(0.85f, 1.15f);
        UGameplayStatics::PlaySoundAtLocation(this, SonidoDisparo2, GetOwner()->GetActorLocation(), 1.0f, PitchAleatorio);
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
    if (SonidoSalto && GetOwner()) UGameplayStatics::PlaySoundAtLocation(this, SonidoSalto, GetOwner()->GetActorLocation());
}

void UAudioManager::PlaySoundBoss()
{
    if (SonidoBoss && GetOwner()) UGameplayStatics::PlaySoundAtLocation(this, SonidoBoss, GetOwner()->GetActorLocation());
}

void UAudioManager::PlaySoundImpacto()
{
    if (SonidoImpacto && GetOwner())
    {
        float PitchAleatorio = FMath::RandRange(0.9f, 1.1f);
        UGameplayStatics::PlaySoundAtLocation(this, SonidoImpacto, GetOwner()->GetActorLocation(), 1.0f, PitchAleatorio);
    }
}

void UAudioManager::PlaySoundMusicaFondo()
{
    if (MusicaFondo && MusicaComponent)
    {
        // Si no está sonando, lo configuramos y lo ponemos en Play
        if (!MusicaComponent->IsPlaying())
        {
            MusicaComponent->SetSound(MusicaFondo);
            MusicaComponent->Play();
        }
    }
}

void UAudioManager::PlaySoundRecogerNucleo()
{
    if (SonidoNucleo && GetOwner()) UGameplayStatics::PlaySoundAtLocation(this, SonidoNucleo, GetOwner()->GetActorLocation());
}

void UAudioManager::PlaySoundDerrota()
{
    if (SonidoDerrota) UGameplayStatics::PlaySound2D(this, SonidoDerrota, 1.2f);
}

void UAudioManager::PlaySoundMenosVida()
{
    if (SonidoMenosVida) UGameplayStatics::PlaySound2D(this, SonidoMenosVida, 1.0f);
}

void UAudioManager::PlaySoundChoqueEnemigo()
{
    if (SonidoChoqueEnemigo && GetOwner())
    {
        float PitchAleatorio = FMath::RandRange(0.9f, 1.1f);
        UGameplayStatics::PlaySoundAtLocation(this, SonidoChoqueEnemigo, GetOwner()->GetActorLocation(), 1.0f, PitchAleatorio);
    }
}