#include "DronSuicida.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"
#include "GameAssets.h"

ADronSuicida::ADronSuicida()
{
    // Configurar esfera heredada
    EsferaColision->SetSphereRadius(50.0f);
    EsferaColision->SetGenerateOverlapEvents(true);

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    Malla->SetupAttachment(RootComponent);
    Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(GameAssets::MallaDronSuicida);
    if (SphereMesh.Succeeded())
    {
        Malla->SetStaticMesh(SphereMesh.Object);
        Malla->SetRelativeScale3D(FVector(0.6f));
    }

    VelocidadCarga = 700.0f;
    DistanciaDeteccion = 2000.0f;
    DanoExplosion = 50.0f;
    bActivado = false;
}

void ADronSuicida::BeginPlay()
{
    Super::BeginPlay();
    Objetivo = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ADronSuicida::AlImpactar);
}

void ADronSuicida::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Objetivo) return;

    float Dist = FVector::Dist(GetActorLocation(), Objetivo->GetActorLocation());
    if (Dist < DistanciaDeteccion || bActivado)
    {
        bActivado = true;
        FVector Dir = (Objetivo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorRotation(Dir.Rotation());
        AddActorWorldOffset(Dir * VelocidadCarga * DeltaTime);
    }
}

void ADronSuicida::AlImpactar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
    if (Jugador)
    {
        Jugador->RecibirDano(DanoExplosion);
        Destroy();
    }
}