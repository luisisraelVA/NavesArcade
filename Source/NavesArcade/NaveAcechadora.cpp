#include "NaveAcechadora.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"
#include "Proyectil.h"
#include "GameAssets.h"
#include "Components/SphereComponent.h"

ANaveAcechadora::ANaveAcechadora()
{
    
    EsferaColision->SetSphereRadius(80.0f);

    MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
    MallaEnemigo->SetupAttachment(RootComponent);
    MallaEnemigo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MallaEnemigo->SetGenerateOverlapEvents(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(GameAssets::MallaAcechadora);
    if (MeshObj.Succeeded())
    {
        MallaEnemigo->SetStaticMesh(MeshObj.Object);
        MallaEnemigo->SetRelativeScale3D(FVector(0.3f));
        MallaEnemigo->SetRelativeRotation(FRotator(0, 90, 0));
    }

    VelocidadPersecucion = 350.0f;
    CadenciaDisparo = 1.2f;
    Salud = 30.0f;
    Tags.Add(FName("Enemy"));
    TargetJugador = nullptr;
}

void ANaveAcechadora::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!TargetJugador) TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!TargetJugador) return;

    float Distancia = FVector::Dist(GetActorLocation(), TargetJugador->GetActorLocation());

    FVector Direccion = (TargetJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FRotator RotacionObjetivo = Direccion.Rotation();
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, 4.0f));


    if (Distancia > 800.0f)
    {
        AddActorWorldOffset(GetActorForwardVector() * VelocidadPersecucion * DeltaTime);
    }
    else
    {
        AddActorWorldOffset(GetActorForwardVector() * (VelocidadPersecucion * 1.2f) * DeltaTime);
    }

    AddActorWorldOffset(FVector(0.0f, 0.0f, FMath::Sin(GetWorld()->GetTimeSeconds() * 8.0f) * 60.0f * DeltaTime));

    if (Distancia < 600.0f)
    {
        ANaveJugador* Jugador = Cast<ANaveJugador>(TargetJugador);
        if (Jugador)
        {
            Jugador->RecibirDano(25.0f);
            Destroy();
        }
    }
}


void ANaveAcechadora::BeginPlay()
{
    Super::BeginPlay();         // registra en GameMode
    TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (TargetJugador)
    {
        GetWorldTimerManager().SetTimer(TimerDisparo, this, &ANaveAcechadora::Disparar, CadenciaDisparo, true);
    }
}
void ANaveAcechadora::Disparar()
{
    if (!TargetJugador) return;

    FVector Direccion = (TargetJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Origen = GetActorLocation() + Direccion * 150.0f;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;          
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Direccion.Rotation(), SpawnParams);
}

void ANaveAcechadora::Destroyed()
{
    GetWorldTimerManager().ClearTimer(TimerDisparo);
    Super::Destroyed();  
}