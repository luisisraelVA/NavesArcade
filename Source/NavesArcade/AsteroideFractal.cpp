#include "AsteroideFractal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "NaveJugador.h"

AAsteroideFractal::AAsteroideFractal()
{
    PrimaryActorTick.bCanEverTick = true;

    DanoPorChoque = 30.0f;
    ClaseFragmento = AAsteroideBase::StaticClass();
}

void AAsteroideFractal::BeginPlay()
{
    Super::BeginPlay();
}

void AAsteroideFractal::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
    if (Jugador)
    {
        Jugador->RecibirDano(DanoPorChoque);
        Fragmentar();
        Destroy();
    }
}

void AAsteroideFractal::Fragmentar()
{
    if (!GetWorld() || !ClaseFragmento) return;

    for (int32 i = 0; i < FragmentosHijos; ++i)
    {
        FVector Offset = FMath::VRand() * 200.0f;
        FVector Pos = GetActorLocation() + Offset;
        FRotator Rot = FRotator(FMath::RandRange(-180.f, 180.f), FMath::RandRange(-180.f, 180.f), 0.f);

        AAsteroideBase* Hijo = GetWorld()->SpawnActor<AAsteroideBase>(ClaseFragmento, Pos, Rot);
        if (Hijo)
        {
            Hijo->SetLifeSpan(8.0f);
        }
    }
}