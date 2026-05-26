#include "WeaponSystem.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h" 
#include "NaveJugador.h"
#include "AsteroideBase.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h"

UWeaponSystem::UWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
    CadenciaDisparo = 0.3f;
    bPuedeDisparar = true;
    UltimoTiempoDisparo = 0.0f;
}

void UWeaponSystem::BeginPlay()
{
    Super::BeginPlay();
}

void UWeaponSystem::Disparar(FVector Ubicacion, FRotator Rotacion)
{
    if (!ClaseProyectil) return;

    UWorld* World = GetWorld();
    if (World)
    {
        float TiempoActual = World->GetTimeSeconds();
        if (TiempoActual < UltimoTiempoDisparo) UltimoTiempoDisparo = 0.0f;
        if (TiempoActual - UltimoTiempoDisparo < CadenciaDisparo) return;
        UltimoTiempoDisparo = TiempoActual;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.Instigator = Cast<APawn>(GetOwner());
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        World->SpawnActor<AProyectil>(ClaseProyectil, Ubicacion, Rotacion, SpawnParams);
    }
}

void UWeaponSystem::ResetearDisparo()
{
}