#include "DronHibridoAvanzado.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Proyectil.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"
#include "LevelBuilder.h"

ADronHibridoAvanzado::ADronHibridoAvanzado()
{
    PrimaryActorTick.bCanEverTick = true;

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    RootComponent = Malla;
    Malla->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Malla->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshAsset.Succeeded())
    {
        Malla->SetStaticMesh(MeshAsset.Object);
        Malla->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
    }

    Tags.Add(FName("Enemy"));
}

void ADronHibridoAvanzado::BeginPlay()
{
    Super::BeginPlay();
    Objetivo = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetWorldTimerManager().SetTimer(TimerDisparo, this, &ADronHibridoAvanzado::DispararRafaga, 2.0f, true, 1.0f);

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
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
        GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Rot, Params);
    }
}

void ADronHibridoAvanzado::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}

void ADronHibridoAvanzado::RecibirDano(float Cantidad)
{
    Destroy();
}