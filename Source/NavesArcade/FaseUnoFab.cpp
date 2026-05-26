#include "FaseUnoFab.h"
#include "EnemyFactory.h"

AActor* UFaseUnoFab::CrearEnemigo(UWorld* World, FVector Location)
{
    return UEnemyFactory::SpawnEnemy(World, EEnemyType::Sentry, Location);
}