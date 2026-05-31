#include "DronSuicida.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Proyectil.h"
#include "NaveJugador.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"

ADronSuicida::ADronSuicida()
{
    PrimaryActorTick.bCanEverTick = true;

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    RootComponent = Malla;
    Malla->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Malla->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConoMesh(TEXT("StaticMesh'/Game/MESH/hal2_Box001.hal2_Box001'"));
    if (ConoMesh.Succeeded())
    {
        Malla->SetStaticMesh(ConoMesh.Object);
        Malla->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
    }

    // Colisión para daño por contacto (opcional, si quieres que aún dañe al tocarte)
    ColisionDano = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionDano"));
    ColisionDano->SetupAttachment(RootComponent);
    ColisionDano->InitSphereRadius(150.0f);
    ColisionDano->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ColisionDano->SetGenerateOverlapEvents(true);

    CadenciaDisparo = 1.0f;
    Tags.Add(FName("Enemy"));
}

void ADronSuicida::BeginPlay()
{
    Super::BeginPlay();
    Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Disparo automático cada ciertos segundos
    GetWorldTimerManager().SetTimer(TimerDisparo, this, &ADronSuicida::Disparar, CadenciaDisparo, true);

    // Registrar para HUD
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
}

void ADronSuicida::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Jugador) return;

    // Solo rotar hacia el jugador para apuntar (sin movimiento)
    FVector Direccion = (Jugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FRotator Rotacion = Direccion.Rotation();
    SetActorRotation(Rotacion);
}

void ADronSuicida::Disparar()
{
    if (!Jugador) return;
    FVector Dir = (Jugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Origen = GetActorLocation() + Dir * 150.0f;
    FActorSpawnParameters Params;
    Params.Owner = this;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Dir.Rotation(), Params);
}

void ADronSuicida::RecibirDano(float Cantidad)
{
    // Destruir al recibir daño (de los disparos del jugador)
    Destroy();
}

void ADronSuicida::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}