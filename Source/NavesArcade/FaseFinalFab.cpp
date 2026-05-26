#include "FaseFinalFab.h"
#include "EnemyFactory.h"

AActor* UFaseFinalFab::CrearEnemigo(UWorld* World, FVector Location)
{
    return UEnemyFactory::SpawnEnemy(World, EEnemyType::Hunter, Location);
}