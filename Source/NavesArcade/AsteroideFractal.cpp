#include "AsteroideFractal.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Proyectil.h"

AAsteroideFractal::AAsteroideFractal()
{
    PrimaryActorTick.bCanEverTick = true;

    bEsFragmento = false;
    EscalaHijos = 0.6f;

    VelocidadMovimiento = 600.0f;
}

void AAsteroideFractal::BeginPlay()
{
    Super::BeginPlay();

    if (EsferaColision)
    {
        // Quitar lógica base
        EsferaColision->OnComponentBeginOverlap.RemoveDynamic(
            this,
            &AAsteroideFractal::AlSuperponerse
        );

        // Agregar lógica fractal
        EsferaColision->OnComponentBeginOverlap.AddDynamic(
            this,
            &AAsteroideFractal::AlRecibirImpactoProyectil
        );
    }
}

void AAsteroideFractal::AlRecibirImpactoProyectil(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    // Si el impacto fue un proyectil
    if (OtherActor->IsA(AProyectil::StaticClass()))
    {
        // Solo dividir el asteroide padre
        if (!bEsFragmento && GetWorld())
        {
            FVector UbicacionActual = GetActorLocation();

            FVector DirIzquierda =
                DireccionMovimiento.RotateAngleAxis(
                    -90.0f,
                    FVector(0.0f, 0.0f, 1.0f)
                );

            FVector DirDerecha =
                DireccionMovimiento.RotateAngleAxis(
                    90.0f,
                    FVector(0.0f, 0.0f, 1.0f)
                );

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            // HIJO IZQUIERDO
            AAsteroideFractal* HijoIzq =
                GetWorld()->SpawnActor<AAsteroideFractal>(
                    AAsteroideFractal::StaticClass(),
                    UbicacionActual,
                    FRotator::ZeroRotator,
                    SpawnParams
                );

            if (HijoIzq)
            {
                HijoIzq->bEsFragmento = true;

                HijoIzq->SetActorScale3D(
                    GetActorScale3D() * EscalaHijos
                );

                HijoIzq->ConfigurarMovimiento(
                    VelocidadMovimiento * 1.3f,
                    DirIzquierda
                );

                HijoIzq->SetLifeSpan(6.0f);
            }

            // HIJO DERECHO
            AAsteroideFractal* HijoDer =
                GetWorld()->SpawnActor<AAsteroideFractal>(
                    AAsteroideFractal::StaticClass(),
                    UbicacionActual,
                    FRotator::ZeroRotator,
                    SpawnParams
                );

            if (HijoDer)
            {
                HijoDer->bEsFragmento = true;

                HijoDer->SetActorScale3D(
                    GetActorScale3D() * EscalaHijos
                );

                HijoDer->ConfigurarMovimiento(
                    VelocidadMovimiento * 1.3f,
                    DirDerecha
                );

                HijoDer->SetLifeSpan(6.0f);
            }

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    2.f,
                    FColor::Yellow,
                    TEXT("FRACTAL: Asteroide dividido")
                );
            }
        }

        OtherActor->Destroy();
        Destroy();
    }
    else
    {
        // comportamiento base normal
        Super::AlSuperponerse(
            OverlappedComponent,
            OtherActor,
            OtherComp,
            OtherBodyIndex,
            bFromSweep,
            SweepResult
        );
    }
}