#include "NaveJugador.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "InventoryComponent.h"
#include "WeaponSystem.h" // Incluimos el header para acceder a sus funciones
#include "NaveFacade.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Proyectil.h"
#include "PortalSalto.h"
#include "LevelBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NavesArcadeGameMode.h"

ANaveJugador::ANaveJugador()
{
    PrimaryActorTick.bCanEverTick = true;
    MallaNave = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNave"));
    RootComponent = MallaNave;
    MallaNave->SetEnableGravity(false);
    MallaNave->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ModeloNaveAsset(TEXT("StaticMesh'/Game/Flying/Meshes/UFO.UFO'"));
    if (ModeloNaveAsset.Succeeded()) MallaNave->SetStaticMesh(ModeloNaveAsset.Object);

    BrazoCamara = CreateDefaultSubobject<USpringArmComponent>(TEXT("BrazoCamara"));
    BrazoCamara->SetupAttachment(RootComponent);
    BrazoCamara->TargetArmLength = 500.0f;
    BrazoCamara->bEnableCameraLag = true;
    BrazoCamara->CameraLagSpeed = 3.0f;
    BrazoCamara->bEnableCameraRotationLag = true;
    BrazoCamara->CameraRotationLagSpeed = 3.0f;

    CamaraNave = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraNave"));
    CamaraNave->SetupAttachment(BrazoCamara);

    Inventario = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventario"));
    FachadaNave = CreateDefaultSubobject<UNaveFacade>(TEXT("FachadaNave"));

    // CORREGIDO: Inicialización real de la variable del componente de armas
    SistemaArmas = CreateDefaultSubobject<UWeaponSystem>(TEXT("SistemaArmas"));

    IntegridadEstructural = 100.0f;
    VelocidadMovimiento = 1000.0f;
    VelocidadRotacion = 85.0f;
    VidasActuales = 3;
    PuntuacionTotal = 0;
    MultiplicadorCombo = 1.0f;
    TiempoTemblorCamara = 0.0f;
    NucleosRecolectados = 0;
    EnergiaActual = 0.0f;

    ClaseProyectil = AProyectil::StaticClass();
}

void ANaveJugador::BeginPlay()
{
    Super::BeginPlay();

    ANavesArcadeGameMode* GameMode = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (Inventario && GameMode)
    {
        Inventario->SetRequerimientoNivel(GameMode->GetNucleosRequeridos());
    }
}

void ANaveJugador::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (TiempoTemblorCamara > 0.0f)
    {
        TiempoTemblorCamara -= DeltaTime;
        BrazoCamara->SocketOffset = FVector(FMath::RandRange(-20.f, 20.f), FMath::RandRange(-20.f, 20.f), FMath::RandRange(-20.f, 20.f));
    }
    else BrazoCamara->SocketOffset = FVector::ZeroVector;
}

// Parte corregida de NaveJugador.cpp

void ANaveJugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!PlayerInputComponent) return;

    // Movimiento
    PlayerInputComponent->BindAxis("MoverAdelante", this, &ANaveJugador::MoverAdelante);

    // Rotaciones
    PlayerInputComponent->BindAxis("RotarDerecha", this, &ANaveJugador::RotarDerecha);
    PlayerInputComponent->BindAxis("RotarArriba", this, &ANaveJugador::RotarArriba);

    // Disparo
    PlayerInputComponent->BindAction("Disparar", IE_Pressed, this, &ANaveJugador::InicializarDisparo);
}

void ANaveJugador::InicializarDisparo()
{
    if (IntegridadEstructural <= 0.0f) return;
    if (FachadaNave) FachadaNave->EjecutarDisparo();
}

void ANaveJugador::MoverAdelante(float Valor)
{
    if (Valor != 0.0f) AddActorWorldOffset(GetActorForwardVector() * Valor * VelocidadMovimiento * GetWorld()->GetDeltaSeconds(), false);
}

void ANaveJugador::RotarDerecha(float Valor)
{
    if (Valor != 0.0f) AddActorLocalRotation(FRotator(0.0f, Valor * VelocidadRotacion * GetWorld()->GetDeltaSeconds(), 0.0f));
}

void ANaveJugador::RotarArriba(float Valor)
{
    if (Valor != 0.0f) AddActorLocalRotation(FRotator(Valor * VelocidadRotacion * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f));
}

void ANaveJugador::RecibirDano(float CantidadDano)
{
    if (IntegridadEstructural <= 0.0f) return;

    //IntegridadEstructural -= CantidadDano;   // Daño real
    MultiplicadorCombo = 1.0f;
    TiempoTemblorCamara = 0.5f;

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("¡ALERTA! Vida: %.0f"), IntegridadEstructural));

    if (IntegridadEstructural <= 0.0f)
    {
        VidasActuales--;                     // Resta vidas
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("¡NAVE DESTRUIDA! Respawn en 2 segundos..."));

        if (VidasActuales > 0)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC) PC->DisableInput(PC);

            FTimerHandle TimerRespawn;
            GetWorldTimerManager().SetTimer(TimerRespawn, this, &ANaveJugador::Reaparecer, 2.0f, false);
        }
        else
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC) PC->DisableInput(PC);

            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("GAME OVER - Reiniciando nivel..."));
            FTimerHandle TimerReinicio;
            GetWorldTimerManager().SetTimer(TimerReinicio, this, &ANaveJugador::ReiniciarNivel, 3.5f, false);
        }
    }
}

void ANaveJugador::Reaparecer()
{
    SetActorLocation(GetActorLocation() + FVector(-3000.0f, FMath::RandRange(-2000.0f, 2000.0f), 0.0f));
    SetActorRotation(FRotator::ZeroRotator);
    IntegridadEstructural = 100.0f;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC) PC->EnableInput(PC);
}

void ANaveJugador::RecolectarEnergia(float Cantidad)
{
    NucleosRecolectados++;
    EnergiaActual = (float)NucleosRecolectados;
    SumarPuntos(1000);

    ANavesArcadeGameMode* GameMode = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    int32 Requeridos = (Inventario ? Inventario->GetRequerimientoNivel() : (GameMode ? GameMode->GetNucleosRequeridos() : 3));

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Núcleo %d / %d"), NucleosRecolectados, Requeridos));

    if (NucleosRecolectados >= Requeridos)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, TEXT("¡PORTAL ABIERTO! ¡ESCAPA!"));
        FVector UbicacionPortal = GetActorLocation() + (GetActorForwardVector() * 6000.0f);
        GetWorld()->SpawnActor<APortalSalto>(APortalSalto::StaticClass(), UbicacionPortal, FRotator::ZeroRotator);
    }
    else
    {
        AActor* BuilderActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass());
        if (BuilderActor) Cast<ALevelBuilder>(BuilderActor)->GenerarFaseObjetivo();
    }
}

void ANaveJugador::SumarPuntos(int32 PuntosBase)
{
    PuntuacionTotal += (PuntosBase * MultiplicadorCombo);
    MultiplicadorCombo += 0.1f;
}

void ANaveJugador::ReiniciarNivel()
{
    FString NombreMapa = GetWorld()->GetMapName();
    NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    UGameplayStatics::OpenLevel(GetWorld(), FName(*NombreMapa));
}

void ANaveJugador::Destroyed()
{
    Super::Destroyed();
}