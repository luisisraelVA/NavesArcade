#include "NaveAcechadora.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "NaveJugador.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"
#include "Proyectil.h"   // Para spawnear proyectiles

ANaveAcechadora::ANaveAcechadora()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
    RootComponent = MallaEnemigo;
    MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaEnemigo->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConoMesh(TEXT("StaticMesh'/Game/MESH/hal2_Box002.hal2_Box002'"));
    if (ConoMesh.Succeeded())
    {
        MallaEnemigo->SetStaticMesh(ConoMesh.Object);
        MallaEnemigo->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
    }
    else
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("StaticMesh'/Game/MESH/space_mouse_fbx.space_mouse_fbx'"));
        if (CylinderMesh.Succeeded()) MallaEnemigo->SetStaticMesh(CylinderMesh.Object);
    }

    VelocidadPersecucion = 350.0f;
    CadenciaDisparo = 1.2f;   // Dispara cada 1.2 segundos
    Tags.Add(FName("Enemy"));
    TargetJugador = nullptr;
}

void ANaveAcechadora::BeginPlay()
{
    Super::BeginPlay();
    TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Iniciar el temporizador de disparos
    if (TargetJugador)
    {
        GetWorldTimerManager().SetTimer(TimerDisparo, this, &ANaveAcechadora::Disparar, CadenciaDisparo, true);
    }

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
}

void ANaveAcechadora::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!TargetJugador) TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!TargetJugador) return;

    float Distancia = FVector::Dist(GetActorLocation(), TargetJugador->GetActorLocation());

    // Movimiento de persecución
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

    // Movimiento vertical flotante
    AddActorWorldOffset(FVector(0.0f, 0.0f, FMath::Sin(GetWorld()->GetTimeSeconds() * 8.0f) * 60.0f * DeltaTime));

    // ========== ATAQUE KAMIKAZE (muerte al chocar) ==========
    if (Distancia < 600.0f)
    {
        ANaveJugador* Jugador = Cast<ANaveJugador>(TargetJugador);
        if (Jugador)
        {
            Jugador->RecibirDano(25.0f);   // Daño al jugador
            Destroy();                      // La nave acechadora se destruye
        }
    }
}

void ANaveAcechadora::Disparar()
{
    if (!TargetJugador) return;

    // Calcular dirección hacia el jugador
    FVector Direccion = (TargetJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    // Origen del disparo (delante de la nave)
    FVector Origen = GetActorLocation() + Direccion * 150.0f;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Direccion.Rotation(), SpawnParams);
}

void ANaveAcechadora::Destroyed()
{
    // Limpiar el temporizador de disparos
    GetWorldTimerManager().ClearTimer(TimerDisparo);

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}