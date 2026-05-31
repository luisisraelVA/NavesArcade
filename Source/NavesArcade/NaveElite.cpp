#include "NaveElite.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Proyectil.h"
#include "NaveJugador.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"

ANaveElite::ANaveElite()
{
    PrimaryActorTick.bCanEverTick = true;

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    RootComponent = Malla;
    Malla->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Malla->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Game/MESH/Defender-class.Defender-class'"));
    if (CuboMesh.Succeeded())
    {
        Malla->SetStaticMesh(CuboMesh.Object);
        Malla->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
    }

    Escudo = 30.0f;
    Salud = 20.0f;
    Tags.Add(FName("Enemy"));
}

void ANaveElite::BeginPlay()
{
    Super::BeginPlay();
    Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetWorldTimerManager().SetTimer(TimerDisparo, this, &ANaveElite::Disparar, 0.8f, true);

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
}

void ANaveElite::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Jugador) return;

    // Solo rotar hacia el jugador (sin movimiento)
    FVector Direccion = (Jugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FRotator Rotacion = Direccion.Rotation();
    Rotacion.Yaw += 90.0f;
    SetActorRotation(Rotacion);
}

void ANaveElite::Disparar()
{
    if (!Jugador) return;
    FVector Dir = (Jugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Origen = GetActorLocation() + Dir * 200.0f;
    FActorSpawnParameters Params;
    Params.Owner = this;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Dir.Rotation(), Params);
}

void ANaveElite::RecibirDano(float Cantidad)
{
    if (Escudo > 0)
    {
        Escudo -= Cantidad;
        if (Escudo <= 0)
        {
            // Escudo roto
        }
    }
    else
    {
        Salud -= Cantidad;
        if (Salud <= 0) Destroy();
    }
}

void ANaveElite::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}