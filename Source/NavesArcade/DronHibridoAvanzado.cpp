#include "DronHibridoAvanzado.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Proyectil.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"
#include "LevelBuilder.h"
#include "Components/SphereComponent.h"
#include "GameAssets.h"

ADronHibridoAvanzado::ADronHibridoAvanzado()
{
    PrimaryActorTick.bCanEverTick = true;

    // Esfera heredada
    EsferaColision->SetSphereRadius(80.0f);

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    Malla->SetupAttachment(RootComponent);
    Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(GameAssets::MallaDronHibrido);
    if (MeshObj.Succeeded())
    {
        Malla->SetStaticMesh(MeshObj.Object);
        Malla->SetRelativeScale3D(FVector(0.6f));
        Malla->SetRelativeRotation(FRotator(0, 90, 0));
    }

    Salud = 25.0f;
    Tags.Add(FName("Enemy"));
    VelocidadMovimiento = 400.0f;
    DistanciaDisparo = 1200.0f;
}

void ADronHibridoAvanzado::BeginPlay()
{
    Super::BeginPlay();   // registra en GameMode
    Objetivo = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetWorldTimerManager().SetTimer(TimerDisparo, this, &ADronHibridoAvanzado::DispararRafaga, 2.0f, true, 1.0f);
}

void ADronHibridoAvanzado::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Objetivo) return;

    FVector Dir = (Objetivo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    SetActorRotation(Dir.Rotation());
    AddActorWorldOffset(Dir * VelocidadMovimiento * DeltaTime);
}

void ADronHibridoAvanzado::DispararRafaga()
{
    if (!Objetivo || !GetWorld()) return;

    FVector Dir = (Objetivo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Origen = GetActorLocation() + Dir * 200.0f;

    for (int32 i = -1; i <= 1; ++i)
    {
        FRotator Rot = Dir.Rotation();
        Rot.Yaw += i * 15.0f;
        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = this;           // <-- AÑADIR
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Rot, Params);
    }
}

void ADronHibridoAvanzado::Destroyed()
{
    GetWorldTimerManager().ClearTimer(TimerDisparo);
    Super::Destroyed();   // desregistro y notificación automática
}