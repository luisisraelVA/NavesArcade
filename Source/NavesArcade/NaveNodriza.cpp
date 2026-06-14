#include "NaveNodriza.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NaveNodriza.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Proyectil.h"
#include "NaveAcechadora.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"
#include "GameAssets.h"

ANaveNodriza::ANaveNodriza()
{
    PrimaryActorTick.bCanEverTick = true;
    EsferaColision->SetSphereRadius(500.0f);
    EsferaColision->SetCollisionProfileName(TEXT("BlockAll"));
    EsferaColision->SetGenerateOverlapEvents(true);

    MallaJefe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaJefe"));
    MallaJefe->SetupAttachment(RootComponent);
    MallaJefe->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MallaJefe->SetGenerateOverlapEvents(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(GameAssets::MallaNaveNodriza);
    if (MeshObj.Succeeded())
    {
        MallaJefe->SetStaticMesh(MeshObj.Object);
        MallaJefe->SetRelativeScale3D(FVector(5.0f));
        MallaJefe->SetRelativeRotation(FRotator(0, -90, 0));
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(
        TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
    if (ParticulaAsset.Succeeded()) EfectoMuerteMasiva = ParticulaAsset.Object;

    VidaMaxima = 800.0f;
    VidaActual = VidaMaxima;
    bFaseDos = false;
    bMuerto = false;
    AnguloEspiralAcumulado = 0.0f;
    bEsNivelNueve = false;
}


void ANaveNodriza::BeginPlay()
{
    Super::BeginPlay();

    ANavesArcadeGameMode* GameMode = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        switch (GameMode->DificultadActual)
        {
        case EDificultad::Facil: VidaMaxima = 600.0f; break;
        case EDificultad::Medio: VidaMaxima = 800.0f; break;
        case EDificultad::Dificil: VidaMaxima = 1000.0f; break;
        default: VidaMaxima = 800.0f; break;
        }

        if (GameMode->GetNivelActual() == 12)
        {
            VidaMaxima = 400.0f;
            bEsNivelNueve = true;
        }

        VidaActual = VidaMaxima;
    }

    if (bEsNivelNueve)
    {
        GetWorldTimerManager().SetTimer(TimerDisparoAbanico, this, &ANaveNodriza::DispararEspiralHelicoidal, 0.08f, true, 1.0f);
    }
    else
    {
        GetWorldTimerManager().SetTimer(TimerDisparoAbanico, this, &ANaveNodriza::DispararAbanico, 1.5f, true, 1.0f);
    }
}

void ANaveNodriza::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bMuerto) return;

    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Jugador)
    {
        FVector Direccion = (Jugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        AddActorWorldOffset(Direccion * 150.0f * DeltaTime);

        FRotator RotObjetivo = Direccion.Rotation();
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotObjetivo, DeltaTime, 2.0f));
    }

    AddActorWorldOffset(FVector(0.0f, 0.0f, FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f) * 80.0f * DeltaTime));

    if (VidaActual <= VidaMaxima / 2.0f && !bFaseDos)
    {
        bFaseDos = true;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NODRIZA ENFURECIDA!"));

        ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
        if (GM && GM->GetNivelActual() != 12)
        {
            GetWorldTimerManager().SetTimer(TimerInvocacion, this, &ANaveNodriza::InvocarKamikaze, 2.5f, true);
        }
    }
}

void ANaveNodriza::DispararAbanico()
{
    if (bMuerto) return;
    TArray<float> Angulos = { -25.0f, 0.0f, 25.0f };
    for (float Angulo : Angulos)
    {
        FRotator Rotacion = GetActorRotation();
        Rotacion.Yaw += Angulo;
        FVector Origen = GetActorLocation() + Rotacion.Vector() * 600.0f;
        FActorSpawnParameters SpawnParams;
        SpawnParams.Instigator = this;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Rotacion, SpawnParams);
    }
}

void ANaveNodriza::DispararEspiralHelicoidal()
{
    if (bMuerto || !GetWorld()) return;
    AnguloEspiralAcumulado += 15.0f;
    if (AnguloEspiralAcumulado >= 360.0f) AnguloEspiralAcumulado -= 360.0f;
    FRotator RotacionProyectil = GetActorRotation();
    RotacionProyectil.Yaw += AnguloEspiralAcumulado;
    FVector Origen = GetActorLocation() + RotacionProyectil.Vector() * 400.0f;
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, RotacionProyectil, SpawnParams);
}

void ANaveNodriza::InvocarKamikaze()
{
    if (bMuerto) return;
    float Angulo = FMath::RandRange(0.0f, 360.0f);
    float Distancia = FMath::RandRange(400.0f, 700.0f);
    FVector Offset = FVector(
        FMath::Cos(FMath::DegreesToRadians(Angulo)) * Distancia,
        FMath::Sin(FMath::DegreesToRadians(Angulo)) * Distancia,
        FMath::RandRange(-200.0f, 200.0f)
    );

    FVector SpawnLoc = GetActorLocation() + Offset;
    ANaveAcechadora* Kamikaze = GetWorld()->SpawnActor<ANaveAcechadora>(ANaveAcechadora::StaticClass(), SpawnLoc, FRotator::ZeroRotator);
    if (Kamikaze) Kamikaze->SetActorScale3D(FVector(1.2f));
}

void ANaveNodriza::RecibirDano(float Dano)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Nodriza recibe daño: %.1f"), Dano));
    if (bMuerto) return;
    VidaActual -= Dano;

    
    if (VidaActual <= 0.0f)
    {
        bMuerto = true;
        GetWorldTimerManager().ClearAllTimersForObject(this);
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);
        if (EfectoMuerteMasiva)
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoMuerteMasiva, GetActorLocation(), FRotator::ZeroRotator, FVector(12.0f));
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        FTimerDelegate TimerDel;
        TimerDel.BindUFunction(this, FName("FinalizarMuerteCinematica"));
        GetWorldTimerManager().SetTimer(TimerMuerteCinematica, TimerDel, 0.2f, false);
    }
}

void ANaveNodriza::RestaurarMaterial()
{
   
}

void ANaveNodriza::FinalizarMuerteCinematica()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    Destroy();
}

void ANaveNodriza::Destroyed()
{
    Super::Destroyed();
}