#include "NaveJugador.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "InventoryComponent.h"
#include "NaveFacade.h"
#include "Engine/World.h"
#include "Proyectil.h"
#include "PortalSalto.h"
#include "GameAssets.h"
#include "LevelBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NavesArcadeGameMode.h"

ANaveJugador::ANaveJugador()
{
    PrimaryActorTick.bCanEverTick = true;

    // Esfera de colisión raíz (solapamiento: daño, núcleos, portales)
    ColisionNave = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionNave"));
    RootComponent = ColisionNave;
    ColisionNave->InitSphereRadius(80.0f);
    ColisionNave->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ColisionNave->SetGenerateOverlapEvents(true);

    // Esfera de colisión física (bloquea contra la nodriza y escenarios)
    ColisionFisica = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionFisica"));
    ColisionFisica->SetupAttachment(RootComponent);
    ColisionFisica->InitSphereRadius(80.0f);
    ColisionFisica->SetCollisionProfileName(TEXT("Pawn"));        // Bloquea contra otros pawns y objetos estáticos
    ColisionFisica->SetGenerateOverlapEvents(false);

    // Malla esquelética (solo visual, sin colisión)
    MallaNave = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MallaNaveVisual"));
    MallaNave->SetupAttachment(RootComponent);
    MallaNave->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MallaNave->SetGenerateOverlapEvents(false);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> ModeloNaveAsset(GameAssets::MallaNaveJugador);
    if (ModeloNaveAsset.Succeeded())
    {
        MallaNave->SetSkeletalMesh(ModeloNaveAsset.Object);
        MallaNave->SetRelativeRotation(FRotator(0, -90, 0));    // Ajuste de orientación
        MallaNave->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
    }

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

    IntegridadEstructural = 100.0f;
    VelocidadMovimiento = 1000.0f;
    VelocidadRotacion = 85.0f;
    VidasActuales = 3;
    PuntuacionTotal = 0;
    MultiplicadorCombo = 1.0f;
    TiempoTemblorCamara = 0.0f;
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
        if (TiempoTemblorCamara <= 0.0f) BrazoCamara->SocketOffset = FVector::ZeroVector;
    }
}

void ANaveJugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (!PlayerInputComponent) return;

    PlayerInputComponent->BindAxis("MoverAdelante", this, &ANaveJugador::MoverAdelante);
    PlayerInputComponent->BindAxis("RotarDerecha", this, &ANaveJugador::RotarDerecha);
    PlayerInputComponent->BindAxis("RotarArriba", this, &ANaveJugador::RotarArriba);
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

    IntegridadEstructural -= CantidadDano;

    MultiplicadorCombo = 1.0f;
    TiempoTemblorCamara = 0.5f;

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("¡ALERTA! Vida: %.0f"), IntegridadEstructural));

    if (IntegridadEstructural <= 0.0f)
    {
        VidasActuales--;
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("¡NAVE DESTRUIDA! Respawn en 2 segundos..."));

        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC) PC->DisableInput(PC);

        if (VidasActuales > 0)
        {
            FTimerHandle TimerRespawn;
            GetWorldTimerManager().SetTimer(TimerRespawn, this, &ANaveJugador::Reaparecer, 2.0f, false);
        }
        else
        {
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
    if (Inventario) Inventario->AgregarEnergia(Cantidad);
    SumarPuntos(1000);

    ANavesArcadeGameMode* GameMode = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    int32 Requeridos = (Inventario ? Inventario->GetRequerimientoNivel() : (GameMode ? GameMode->GetNucleosRequeridos() : 3));
    int32 Actuales = GetNucleosRecolectados();

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Núcleo %d / %d"), Actuales, Requeridos));

    if (Actuales >= Requeridos)
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

int32 ANaveJugador::GetNucleosRecolectados() const
{
    return Inventario ? (int32)Inventario->GetEnergiaActual() : 0;
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