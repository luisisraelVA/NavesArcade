#include "FaseFinalFab.h"
#include "EnemyFactory.h"

AActor* UFaseFinalFab::CrearEnemigo(UWorld* World, FVector Location)
{
    if (FMath::FRand() < 0.3f)   
        return UEnemyFactory::SpawnEnemy(World, EEnemyType::Elite, Location);
    else
        return UEnemyFactory::SpawnEnemy(World, EEnemyType::Hunter, Location);
}