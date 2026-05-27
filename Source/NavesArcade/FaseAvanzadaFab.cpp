#include "FaseAvanzadaFab.h"
#include "DronHibridoAvanzado.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

// CORREGIDO: Se usa el operador '::' en lugar de ':' para resolver el ámbito de la interfaz
AActor* UFaseAvanzadaFab::CrearEnemigo(UWorld* World, FVector Location)
{
    if (!World) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawneamos el Dron Híbrido Observer
    AActor* NuevoEnemigo = World->SpawnActor<ADronHibridoAvanzado>(ADronHibridoAvanzado::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

    if (NuevoEnemigo)
    {
        if (APawn* EnemigoPawn = Cast<APawn>(NuevoEnemigo))
        {
            EnemigoPawn->SpawnDefaultController();
        }
    }

    return NuevoEnemigo;
}