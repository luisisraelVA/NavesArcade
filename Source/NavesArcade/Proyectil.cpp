#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystem.h" 
#include "AsteroideBase.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h" 
#include "NaveJugador.h"
#include "DronHibridoAvanzado.h"
#include "DronSuicida.h"   // + Añadir
#include "NaveElite.h"

AProyectil::AProyectil()
{
    PrimaryActorTick.bCanEverTick = false;

    EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
    EsferaColision->InitSphereRadius(30.0f);
    EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    EsferaColision->SetGenerateOverlapEvents(true);
    RootComponent = EsferaColision;

    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
    MallaProyectil->SetupAttachment(RootComponent);
    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (MeshAsset.Succeeded())
    {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
        MallaProyectil->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
    if (ParticulaAsset.Succeeded()) EfectoExplosion = ParticulaAsset.Object;

    MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
    MovimientoProyectil->UpdatedComponent = EsferaColision;
    MovimientoProyectil->InitialSpeed = 5000.0f;   // ¡Más rápido!
    MovimientoProyectil->MaxSpeed = 5000.0f;
    MovimientoProyectil->ProjectileGravityScale = 0.0f;

    InitialLifeSpan = 5.0f;   // Vive 5 segundos → alcance 25000 unidades
}

void AProyectil::BeginPlay()
{
    Super::BeginPlay();
    if (EsferaColision) EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectil::AlSuperponerse);
}

void AProyectil::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AProyectil::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

    bool bDisparadoPorJugador = (GetOwner() && GetOwner()->IsA(ANaveJugador::StaticClass()));

    if (bDisparadoPorJugador)
    {
        // Daño al jefe
        if (OtherActor->IsA(ANaveNodriza::StaticClass()))
        {
            ANaveNodriza* Boss = Cast<ANaveNodriza>(OtherActor);
            if (Boss) Boss->RecibirDano(25.0f);
            Destroy();
            return;
        }

        // Daño a asteroides y enemigos normales (incluye los nuevos)
        if (OtherActor->IsA(AAsteroideBase::StaticClass()) ||
            OtherActor->IsA(ADronCentinela::StaticClass()) ||
            OtherActor->IsA(ANaveAcechadora::StaticClass()) ||
            OtherActor->IsA(ADronHibridoAvanzado::StaticClass()) ||
            OtherActor->IsA(ADronSuicida::StaticClass()) ||     // + DronSuicida
            OtherActor->IsA(ANaveElite::StaticClass()))         // + NaveElite
        {
            if (EfectoExplosion) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoExplosion, OtherActor->GetActorLocation());
            ANaveJugador* Jugador = Cast<ANaveJugador>(GetOwner());
            if (Jugador) Jugador->SumarPuntos(100);

            // Manejo específico por tipo
            if (ANaveElite* Elite = Cast<ANaveElite>(OtherActor))
            {
                Elite->RecibirDano(10.0f);
            }
            else if (ADronSuicida* Suicida = Cast<ADronSuicida>(OtherActor))
            {
                Suicida->RecibirDano(10.0f);
            }
            else
            {
                OtherActor->Destroy();
            }
            Destroy();
        }
    }
    else // Disparo enemigo
    {
        ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
        if (Jugador)
        {
            Jugador->RecibirDano(15.0f);
            Destroy();
        }
        else if (OtherActor->IsA(AAsteroideBase::StaticClass()))
        {
            Destroy();
        }
    }
}