#include "NaveNodriza.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Proyectil.h"
#include "NaveAcechadora.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"

ANaveNodriza::ANaveNodriza()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaJefe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaJefe"));
    RootComponent = MallaJefe;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (CubeMesh.Succeeded())
    {
        MallaJefe->SetStaticMesh(CubeMesh.Object);
        MallaJefe->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
    }
    SetActorScale3D(FVector(5.0f, 5.0f, 5.0f));

    // ========== VISIBILIDAD Y MATERIAL ==========
    MallaJefe->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));
    MallaJefe->SetVisibility(true);
    SetActorHiddenInGame(false);
    // ============================================

    MallaJefe->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaJefe->SetGenerateOverlapEvents(true);
    MallaJefe->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    VidaMaxima = 800.0f;
    VidaActual = VidaMaxima;
    bFaseDos = false;
    bMuerto = false;
    AnguloEspiralAcumulado = 0.0f;
    bEsNivelNueve = false;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
    if (ParticulaAsset.Succeeded()) EfectoMuerteMasiva = ParticulaAsset.Object;
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
        VidaActual = VidaMaxima;
        if (GameMode->GetNivelActual() == 9) bEsNivelNueve = true;
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
        GetWorldTimerManager().SetTimer(TimerInvocacion, this, &ANaveNodriza::InvocarKamikaze, 2.5f, true);
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
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, RotacionProyectil, SpawnParams);
}

void ANaveNodriza::InvocarKamikaze()
{
    if (bMuerto) return;
    FVector SpawnLoc = GetActorLocation() + FVector(FMath::RandRange(-500.0f, 500.0f), 0.0f, -600.0f);
    ANaveAcechadora* Kamikaze = GetWorld()->SpawnActor<ANaveAcechadora>(ANaveAcechadora::StaticClass(), SpawnLoc, FRotator::ZeroRotator);
    if (Kamikaze) Kamikaze->SetActorScale3D(FVector(1.2f, 1.2f, 1.2f));
}

void ANaveNodriza::RecibirDano(float Dano)
{
    if (bMuerto) return;
    VidaActual -= Dano;
    SetActorScale3D(FVector(4.8f, 4.8f, 4.8f));
    GetWorldTimerManager().SetTimer(TimerHitFlash, this, &ANaveNodriza::RestaurarMaterial, 0.1f, false);
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
    if (!bMuerto) SetActorScale3D(FVector(5.0f, 5.0f, 5.0f));
}

void ANaveNodriza::FinalizarMuerteCinematica()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    Destroy();
}

void ANaveNodriza::Destroyed()
{
    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();
    Super::Destroyed();
}