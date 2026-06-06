#include "DronCentinela.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Proyectil.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameAssets.h"
#include "Components/SphereComponent.h"

ADronCentinela::ADronCentinela()
{
    // La esfera raíz ya existe (heredada de EnemigoBase), solo ajustamos radio
    EsferaColision->SetSphereRadius(80.0f);   // tamaño adecuado

    // Malla visual (sin colisiones)
    MallaDron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaDron"));
    MallaDron->SetupAttachment(RootComponent);
    MallaDron->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MallaDron->SetGenerateOverlapEvents(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaDronAsset(GameAssets::MallaDronCentinela);
    if (MallaDronAsset.Succeeded())
    {
        MallaDron->SetStaticMesh(MallaDronAsset.Object);
        MallaDron->SetRelativeScale3D(FVector(0.2f));
        MallaDron->SetRelativeRotation(FRotator(0, -90, 0));
    }

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
        FRotator RotacionObjetivo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ObjetivoActual->GetActorLocation());
        RotacionObjetivo.Yaw += 90.0f;
        MallaDron->SetWorldRotation(RotacionObjetivo);
    }
}

void ADronCentinela::EjecutarDisparoLaser()
{
    if (!ObjetivoActual) return;
    FVector Direccion = (ObjetivoActual->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Origen = GetActorLocation() + Direccion * 150.0f;

    FActorSpawnParameters Params;
    Params.Owner = this;                    // <-- AÑADIR
    Params.Instigator = this;               // <-- AÑADIR
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Direccion.Rotation(), Params);
}

void ADronCentinela::Destroyed()
{
    GetWorldTimerManager().ClearTimer(TimerCicloDisparo);
    Super::Destroyed();
}