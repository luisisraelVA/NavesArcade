#include "LevelBuilder.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "NucleoEnergia.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ALevelBuilder::ALevelBuilder()
{
    PrimaryActorTick.bCanEverTick = false;

    ClaseAsteroide = AAsteroideDinamico::StaticClass();
    ClaseEnergia = ANucleoEnergia::StaticClass();

    DensidadAsteroides = 4;
    VelocidadGlobal = 500.0f;

    FaseActualMision = 0;
    EnemigosVivosEnSector = 0;
    bJefeAparecido = false;
    bJefeDerrotado = false;
    bNucleoPendiente = false;
}

void ALevelBuilder::BeginPlay() { Super::BeginPlay(); }

void ALevelBuilder::SetClaseAsteroide(TSubclassOf<AAsteroideBase> NuevaClase) { ClaseAsteroide = NuevaClase; }

void ALevelBuilder::SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide)
{
    DensidadAsteroides = FMath::Clamp(Cantidad / 25, 2, 5);
    VelocidadGlobal = VelocidadAsteroide;
    GetWorld()->GetTimerManager().SetTimer(TimerMundoAbierto, this, &ALevelBuilder::GeneracionContinua, 0.6f, true);
    GenerarFaseObjetivo();
}

void ALevelBuilder::GenerarFaseObjetivo()
{
    if (OnGenerarFase.IsBound())
    {
        OnGenerarFase.Execute(FaseActualMision);
    }
}

void ALevelBuilder::RegistrarEnemigos(int32 Cantidad)
{
    EnemigosVivosEnSector += Cantidad;
}

void ALevelBuilder::NotificarMuerteEnemigo()
{
    if (EnemigosVivosEnSector > 0)
    {
        EnemigosVivosEnSector--;

        if (EnemigosVivosEnSector <= 0)
        {
            if (bJefeAparecido && !bJefeDerrotado)
            {
                bJefeDerrotado = true;
            }
            SpawnNucleo();
        }
    }
}

void ALevelBuilder::SpawnNucleo()
{
    if (bNucleoPendiente) return;
    bNucleoPendiente = true;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn || !ClaseEnergia) return;

    FVector UbicacionBase = PlayerPawn->GetActorLocation() + (PlayerPawn->GetActorForwardVector() * 8000.0f);
    UbicacionBase += FVector(0.0f, FMath::RandRange(-3000.0f, 3000.0f), FMath::RandRange(-2000.0f, 2000.0f));

    GetWorld()->SpawnActor<ANucleoEnergia>(ClaseEnergia, UbicacionBase, FRotator::ZeroRotator);

    if (OnNucleoRecolectado.IsBound())
    {
        OnNucleoRecolectado.Broadcast();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Orange, TEXT("OBSERVER: Fase completada"));
    }

    FaseActualMision++;
}

void ALevelBuilder::GeneracionContinua()
{
    if (!ClaseAsteroide || !GetWorld()) return;
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector PosicionJugador = PlayerPawn->GetActorLocation();
    FVector Frente = PlayerPawn->GetActorForwardVector();
    FVector Derecha = PlayerPawn->GetActorRightVector();
    FVector Arriba = PlayerPawn->GetActorUpVector();

    for (int32 i = 0; i < DensidadAsteroides; i++)
    {
        FVector Loc = PosicionJugador + (Frente * FMath::RandRange(7000.0f, 9000.0f))
            + (Derecha * FMath::RandRange(-4500.0f, 4500.0f))
            + (Arriba * FMath::RandRange(-3000.0f, 3000.0f));
        FRotator Rot(FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f));
        AActor* NuevoRoca = GetWorld()->SpawnActor<AActor>(ClaseAsteroide, Loc, Rot);
        if (NuevoRoca)
        {
            NuevoRoca->SetLifeSpan(12.0f);
            AAsteroideDinamico* AstDinamico = Cast<AAsteroideDinamico>(NuevoRoca);
            if (AstDinamico) AstDinamico->ConfigurarMovimiento(VelocidadGlobal, Frente * -1.0f);
        }
    }
}