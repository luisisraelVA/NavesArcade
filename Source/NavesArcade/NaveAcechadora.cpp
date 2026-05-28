#include "NaveAcechadora.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyController.h"
#include "NaveJugador.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"

ANaveAcechadora::ANaveAcechadora()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
    RootComponent = MallaEnemigo;
    MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaEnemigo->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConoMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
    if (ConoMesh.Succeeded())
    {
        MallaEnemigo->SetStaticMesh(ConoMesh.Object);
        MallaEnemigo->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
    }
    else
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
        if (CylinderMesh.Succeeded()) MallaEnemigo->SetStaticMesh(CylinderMesh.Object);
    }

    VelocidadPersecucion = 350.0f;
    AIControllerClass = AEnemyController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    Tags.Add(FName("Enemy"));
    TargetJugador = nullptr;
}

void ANaveAcechadora::BeginPlay()
{
    Super::BeginPlay();
    TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
}

void ANaveAcechadora::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!TargetJugador) TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!TargetJugador) return;

    float Distancia = FVector::Dist(GetActorLocation(), TargetJugador->GetActorLocation());
    if (Distancia > 800.0f)
    {
        FVector Direccion = (TargetJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FRotator RotacionObjetivo = Direccion.Rotation();
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, 4.0f));
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
        if (Jugador) Jugador->RecibirDano(25.0f);
        Destroy();
    }
}

void ANaveAcechadora::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}