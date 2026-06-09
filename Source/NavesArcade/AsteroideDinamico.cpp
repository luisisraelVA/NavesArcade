#include "AsteroideDinamico.h"
#include "GameAssets.h"       
#include "Components/StaticMeshComponent.h"

AAsteroideDinamico::AAsteroideDinamico()
{
    PrimaryActorTick.bCanEverTick = true;
    VelocidadMovimiento = 500.0f;
    DireccionMovimiento = FVector(-1.0f, 0.0f, 0.0f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> NuevaMalla(GameAssets::MallaAsteroideDinamico);
    if (NuevaMalla.Succeeded())
    {
        MallaAsteroide->SetStaticMesh(NuevaMalla.Object);
        MallaAsteroide->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));  
      
    }
}

void AAsteroideDinamico::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector Movimiento = DireccionMovimiento.GetSafeNormal() * VelocidadMovimiento * DeltaTime;
    AddActorWorldOffset(Movimiento, true);
}

void AAsteroideDinamico::ConfigurarMovimiento(float NuevaVelocidad, FVector NuevaDireccion)
{
    VelocidadMovimiento = NuevaVelocidad;
    DireccionMovimiento = NuevaDireccion.GetSafeNormal();
}