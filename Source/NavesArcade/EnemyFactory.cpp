#include "EnemyFactory.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

AActor* UEnemyFactory::SpawnEnemy(UWorld* World, EEnemyType Type, FVector Location)
{
    if (!World) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NuevoEnemigo = nullptr;

    switch (Type)
    {
    case EEnemyType::Sentry:
        NuevoEnemigo = World->SpawnActor<ADronCentinela>(ADronCentinela::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
        break;
    case EEnemyType::Hunter:
        NuevoEnemigo = World->SpawnActor<ANaveAcechadora>(ANaveAcechadora::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
        break;
    case EEnemyType::Boss:
        NuevoEnemigo = World->SpawnActor<ANaveNodriza>(ANaveNodriza::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
        break;
    }

    if (NuevoEnemigo)
    {
        if (APawn* EnemigoPawn = Cast<APawn>(NuevoEnemigo))
        {
            EnemigoPawn->SpawnDefaultController();
        }
    }

    return NuevoEnemigo;
}