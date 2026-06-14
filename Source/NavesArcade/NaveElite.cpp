#include "NaveElite.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Proyectil.h"
#include "Sound/SoundBase.h"
#include "GameAssets.h"
#include "Components/SphereComponent.h"

ANaveElite::ANaveElite()
{
    EsferaColision->SetSphereRadius(80.0f);

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    Malla->SetupAttachment(RootComponent);
    Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(GameAssets::MallaNaveElite);
    if (CuboMesh.Succeeded())
    {
        Malla->SetStaticMesh(CuboMesh.Object);
        Malla->SetRelativeScale3D(FVector(1.0f));
        Malla->SetRelativeRotation(FRotator(0, 0, 0));
    }

    Escudo = 30.0f;
    Salud = 20.0f;

    // Cargamos el sonido del disparo enemigo
    static ConstructorHelpers::FObjectFinder<USoundBase> AudioDisparoObj(TEXT("SoundWave'/Game/Sonidos/DisparoEnemigos.DisparoEnemigos'"));
    if (AudioDisparoObj.Succeeded())
    {
        SonidoDisparoEnemigo = AudioDisparoObj.Object;
    }
}

void ANaveElite::BeginPlay()
{
    Super::BeginPlay();
    Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetWorldTimerManager().SetTimer(TimerDisparo, this, &ANaveElite::Disparar, 0.8f, true);
}

void ANaveElite::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Jugador) return;

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

    // Reproducimos el sonido al disparar
    if (SonidoDisparoEnemigo)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SonidoDisparoEnemigo, GetActorLocation());
    }

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = this;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Dir.Rotation(), Params);
}

void ANaveElite::Destroyed()
{
    GetWorldTimerManager().ClearTimer(TimerDisparo);
    Super::Destroyed();
}