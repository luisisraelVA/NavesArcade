#include "DronSuicida.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"
#include "LevelBuilder.h"

ADronSuicida::ADronSuicida()
{
    PrimaryActorTick.bCanEverTick = true;

    EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
    RootComponent = EsferaColision;
    EsferaColision->InitSphereRadius(50.0f);
    EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    EsferaColision->SetGenerateOverlapEvents(true);

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    Malla->SetupAttachment(RootComponent);
    Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        Malla->SetStaticMesh(SphereMesh.Object);
        Malla->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
    }

    Tags.Add(FName("Enemy"));
}

void ADronSuicida::BeginPlay()
{
    Super::BeginPlay();

    Objetivo = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ADronSuicida::AlImpactar);

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
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

void ADronSuicida::AlImpactar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
    if (Jugador)
    {
        Jugador->RecibirDano(DanoExplosion);
        Destroy();
    }
}

void ADronSuicida::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}

void ADronSuicida::RecibirDano(float Cantidad)
{
    Destroy();
}