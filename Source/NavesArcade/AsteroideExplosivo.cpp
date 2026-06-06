#include "AsteroideExplosivo.h"
#include "Components/StaticMeshComponent.h"

AAsteroideExplosivo::AAsteroideExplosivo()
{
    PrimaryActorTick.bCanEverTick = true;

    DanoPorChoque = 50.0f;
    VelocidadMovimiento = 1100.0f;
}

void AAsteroideExplosivo::BeginPlay()
{
    Super::BeginPlay();
}