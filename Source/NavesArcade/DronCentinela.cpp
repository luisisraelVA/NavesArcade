#include "DronCentinela.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"
#include "Proyectil.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelBuilder.h"
#include "Engine/StaticMesh.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

ADronCentinela::ADronCentinela()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaDron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaDron"));
    RootComponent = MallaDron;
    MallaDron->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaDron->SetGenerateOverlapEvents(true);
    MallaDron->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Intentar cargar el modelo personalizado
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaDronAsset(TEXT("StaticMesh'/Game/Meshes/Enemies/nave-modelo.nave-modelo'"));
    if (MallaDronAsset.Succeeded())
    {
        MallaDron->SetStaticMesh(MallaDronAsset.Object);
        MallaDron->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
        UE_LOG(LogTemp, Log, TEXT("Modelo cargado correctamente"));
    }
    else
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> EsferaMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
        if (EsferaMesh.Succeeded())
        {
            MallaDron->SetStaticMesh(EsferaMesh.Object);
            MallaDron->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
            UE_LOG(LogTemp, Warning, TEXT("No se encontró el modelo, usando esfera"));
        }
    }

    // Etiqueta para el HUD
    Tags.Add(FName("Enemy"));

    VelocidadPatrulla = 300.0f;
    ObjetivoActual = nullptr;
}

void ADronCentinela::BeginPlay()
{
    Super::BeginPlay();
    ObjetivoActual = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (ObjetivoActual)
    {
        GetWorldTimerManager().SetTimer(TimerCicloDisparo, this, &ADronCentinela::EjecutarDisparoLaser, 3.0f, true, 1.0f);
    }
}

void ADronCentinela::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!ObjetivoActual) ObjetivoActual = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (ObjetivoActual)
    {
        // Calcular rotación hacia el jugador (para la malla)
        FRotator RotacionObjetivo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ObjetivoActual->GetActorLocation());

        // Ajustar el modelo si está desalineado (prueba con 90, -90, 180, etc.)
        // Por defecto, el modelo mira a la derecha, entonces sumamos 90 grados para que mire al frente.
        RotacionObjetivo.Yaw += 90.0f;  // Cambia este valor si es necesario

        // Aplicar rotación solo a la malla (el actor no se rota)
        MallaDron->SetWorldRotation(RotacionObjetivo);
    }
}

void ADronCentinela::EjecutarDisparoLaser()
{
    if (ObjetivoActual)
    {
        // Dirección hacia el jugador (el proyectil irá seguro)
        FVector Direccion = (ObjetivoActual->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector Origen = GetActorLocation() + Direccion * 150.0f;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = Cast<APawn>(this);
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Direccion.Rotation(), SpawnParams);
    }
}

void ADronCentinela::Destroyed()
{
    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder)
    {
        Builder->NotificarMuerteEnemigo();
    }
    Super::Destroyed();
}