#include "AsteroideExplosivo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AAsteroideExplosivo::AAsteroideExplosivo()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (CuboMesh.Succeeded() && MallaAsteroide)
    {
        MallaAsteroide->SetStaticMesh(CuboMesh.Object);
        MallaAsteroide->SetRelativeScale3D(FVector(2.2f, 2.2f, 2.2f));
    }

    DanoPorChoque = 50.0f;
    VelocidadMovimiento = 1100.0f;
}

void AAsteroideExplosivo::BeginPlay()
{
    Super::BeginPlay();
}