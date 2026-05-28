#include "DronHibridoAvanzado.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Proyectil.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"

ADronHibridoAvanzado::ADronHibridoAvanzado()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaDron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaDron"));
    RootComponent = MallaDron;

    MallaDron->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaDron->SetGenerateOverlapEvents(true);
    MallaDron->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Mesh por defecto
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Game/MESH/SpaceShip.SpaceShip'")
    );

    if (MeshAsset.Succeeded())
    {
        MallaDron->SetStaticMesh(MeshAsset.Object);
        MallaDron->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
    }

    Tags.Add(FName("Enemy"));

    VelocidadActual = 400.0f;
    VelocidadFrenetica = 800.0f;
    DireccionPatrulla = FVector(0.0f, 1.0f, 0.0f);

    ObjetivoJugador = nullptr;   // Inicialización segura
}

void ADronHibridoAvanzado::BeginPlay()
{
    Super::BeginPlay();

    ObjetivoJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (ObjetivoJugador)
    {
        GetWorldTimerManager().SetTimer(
            TimerCicloDisparo,
            this,
            &ADronHibridoAvanzado::EjecutarDisparoLaser,
            2.0f,
            true,
            1.0f
        );
    }

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);

    // Observer Pattern
    AActor* BuilderActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass());
    if (BuilderActor)
    {
        ALevelBuilder* Builder = Cast<ALevelBuilder>(BuilderActor);
        if (Builder)
        {
            Builder->OnNucleoRecolectado.AddDynamic(this, &ADronHibridoAvanzado::EscucharAlertaNucleo);
        }
    }
}

void ADronHibridoAvanzado::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Desplazamiento = DireccionPatrulla * VelocidadActual * DeltaTime;
    AddActorWorldOffset(Desplazamiento, true);

    if (FMath::Abs(GetActorLocation().Y) > 2500.0f)
        DireccionPatrulla *= -1.0f;

    if (!ObjetivoJugador)
        ObjetivoJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (ObjetivoJugador)
    {
        FRotator RotacionHaciaNave = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ObjetivoJugador->GetActorLocation());
        RotacionHaciaNave.Yaw += 90.0f;
        MallaDron->SetWorldRotation(RotacionHaciaNave);
    }
}

void ADronHibridoAvanzado::EjecutarDisparoLaser()
{
    if (!ObjetivoJugador) return;

    FVector Direccion = (ObjetivoJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Origen = GetActorLocation() + (Direccion * 200.0f);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = Cast<APawn>(this);
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), Origen, Direccion.Rotation(), SpawnParams);
}

void ADronHibridoAvanzado::EscucharAlertaNucleo()
{
    VelocidadActual = VelocidadFrenetica;
    GetWorldTimerManager().ClearTimer(TimerCicloDisparo);
    GetWorldTimerManager().SetTimer(TimerCicloDisparo, this, &ADronHibridoAvanzado::EjecutarDisparoLaser, 0.8f, true);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("OBSERVER ACTIVO: Dron Híbrido sobrecargado"));
}

void ADronHibridoAvanzado::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}