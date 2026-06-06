#include "EnemyFactory.h"
#include "EnemigoBase.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h"
#include "DronHibridoAvanzado.h"
#include "DronSuicida.h"
#include "NaveElite.h"
#include "Engine/World.h"

AActor* UEnemyFactory::SpawnEnemy(UWorld* World, EEnemyType Type, FVector Location)
{
    if (!World) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TSubclassOf<AActor> ClaseAInstanciar = nullptr;

    switch (Type)
    {
    case EEnemyType::Sentry:  ClaseAInstanciar = ADronCentinela::StaticClass(); break;
    case EEnemyType::Hunter:  ClaseAInstanciar = ANaveAcechadora::StaticClass(); break;
    case EEnemyType::Boss:    ClaseAInstanciar = ANaveNodriza::StaticClass(); break;
    case EEnemyType::Hybrid:  ClaseAInstanciar = ADronHibridoAvanzado::StaticClass(); break;
    case EEnemyType::Suicide: ClaseAInstanciar = ADronSuicida::StaticClass(); break;
    case EEnemyType::Elite:   ClaseAInstanciar = ANaveElite::StaticClass(); break;
    }

    AActor* NuevoEnemigo = nullptr;
    if (ClaseAInstanciar)
    {
        NuevoEnemigo = World->SpawnActor<AActor>(ClaseAInstanciar, Location, FRotator::ZeroRotator, SpawnParams);
        if (APawn* EnemigoPawn = Cast<APawn>(NuevoEnemigo))
        {
            EnemigoPawn->SpawnDefaultController();
        }
    }

    return NuevoEnemigo;
}