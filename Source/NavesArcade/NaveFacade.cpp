#include "NaveFacade.h"
#include "NaveJugador.h"
#include "InventoryComponent.h"
#include "AudioManager.h"
#include "Proyectil.h"
#include "Engine/World.h"

UNaveFacade::UNaveFacade()
{
    PrimaryComponentTick.bCanEverTick = false;
    AudioManager = CreateDefaultSubobject<UAudioManager>(TEXT("AudioManagerComponent"));
    UltimoTiempoDisparo = 0.0f;
}

void UNaveFacade::BeginPlay()
{
    Super::BeginPlay();
    NaveDuenia = Cast<ANaveJugador>(GetOwner());
    if (NaveDuenia)
    {
        Inventario = NaveDuenia->FindComponentByClass<UInventoryComponent>();
    }
}

void UNaveFacade::ProcesarRecoleccionEnergia(float Cantidad)
{
    if (Inventario)
    {
        Inventario->AgregarEnergia(Cantidad);
        if (AudioManager) AudioManager->PlaySoundSalto();
    }
}

void UNaveFacade::RecibirImpacto(float Dano)
{
    if (NaveDuenia)
    {
        NaveDuenia->RecibirDano(Dano);
        if (AudioManager) AudioManager->PlaySoundExplosion();
    }
}

void UNaveFacade::EjecutarDisparo()
{
    if (!NaveDuenia) NaveDuenia = Cast<ANaveJugador>(GetOwner());
    if (!NaveDuenia || !GetWorld()) return;

    float TiempoActual = GetWorld()->GetTimeSeconds();
    if (TiempoActual < UltimoTiempoDisparo) UltimoTiempoDisparo = 0.0f;
    if (TiempoActual - UltimoTiempoDisparo < 0.3f) return;
    UltimoTiempoDisparo = TiempoActual;

    FVector SpawnLoc = NaveDuenia->GetActorLocation() + (NaveDuenia->GetActorForwardVector() * 300.0f);
    FRotator SpawnRot = NaveDuenia->GetActorRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = NaveDuenia;
    SpawnParams.Instigator = NaveDuenia;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
    if (AudioManager) AudioManager->PlaySoundDisparo();
}

bool UNaveFacade::PuedeSaltarDeNivel() const
{
    if (Inventario) return Inventario->TieneEnergiaSuficiente();
    return false;
}