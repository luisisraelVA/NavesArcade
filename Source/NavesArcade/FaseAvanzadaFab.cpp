#include "FaseAvanzadaFab.h"
#include "DronHibridoAvanzado.h"
#include "DronSuicida.h"
#include "Engine/World.h"
#include "NaveElite.h"

AActor* UFaseAvanzadaFab::CrearEnemigo(UWorld* World, FVector Location)
{
    if (!World) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float Aleatorio = FMath::FRand();   // 0.0 - 1.0

    if (Aleatorio < 0.33f)                                    // 33%
    {
        return World->SpawnActor<ANaveElite>(ANaveElite::StaticClass(), Location, FRotator::ZeroRotator, Params);
    }
    else if (Aleatorio < 0.66f)                               // 33%
    {
        return World->SpawnActor<ADronHibridoAvanzado>(ADronHibridoAvanzado::StaticClass(), Location, FRotator::ZeroRotator, Params);
    }
    else                                                       // 34%
    {
        return World->SpawnActor<ADronSuicida>(ADronSuicida::StaticClass(), Location, FRotator::ZeroRotator, Params);
    }
}