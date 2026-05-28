#include "LevelBuilder.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "NucleoEnergia.h"
#include "EnemyFactory.h"
#include "NaveNodriza.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DronHibridoAvanzado.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"

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

void ALevelBuilder::SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica) { FabricaDeFase = NuevaFabrica; }
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
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (!GM) return;

    int32 NucleosRequeridos = GM->GetNucleosRequeridos();
    int32 NivelActual = GM->GetNivelActual();

    bool bEsUltimaFase = (FaseActualMision == NucleosRequeridos - 1);

    FVector UbicacionBase = PlayerPawn->GetActorLocation() + (PlayerPawn->GetActorForwardVector() * 8000.0f);
    UbicacionBase += FVector(0.0f, FMath::RandRange(-3000.0f, 3000.0f), FMath::RandRange(-2000.0f, 2000.0f));

    int32 NumEnemigos = 0;

    if (bEsUltimaFase)
    {
        bJefeAparecido = true;

        int32 Escoltas = (NivelActual == 9) ? 12 : (3 + (NivelActual - 1));
        NumEnemigos = 1 + Escoltas;

        UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Boss, UbicacionBase + FVector(2000.0f, 0.0f, 0.0f));

        for (int32 i = 0; i < Escoltas; i++)
        {
            FVector PosEnemigo = UbicacionBase + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-1800.f, 1800.f), FMath::RandRange(-1000.f, 1000.f));
            if (NivelActual == 9)
            {
                if (i % 2 == 0)
                    UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Hunter, PosEnemigo);
                else
                    UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, PosEnemigo);
            }
            else
            {
                if (FabricaDeFase)
                {
                    AActor* Enemigo = FabricaDeFase->CrearEnemigo(GetWorld(), PosEnemigo);
                    if (!Enemigo)
                        UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, PosEnemigo);
                }
                else
                {
                    UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, PosEnemigo);
                }
            }
        }
    }
    else
    {
        int32 BaseEnemigos = 3 + FaseActualMision + (NivelActual / 2);
        NumEnemigos = BaseEnemigos;
        for (int32 i = 0; i < NumEnemigos; i++)
        {
            FVector PosEnemigo = UbicacionBase + FVector(FMath::RandRange(-1500.f, 1500.f), FMath::RandRange(-1500.f, 1500.f), FMath::RandRange(-800.f, 800.f));
            if (FabricaDeFase)
            {
                AActor* Enemigo = FabricaDeFase->CrearEnemigo(GetWorld(), PosEnemigo);
                if (!Enemigo)
                    UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, PosEnemigo);
            }
            else
            {
                UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, PosEnemigo);
            }
        }
    }

    EnemigosVivosEnSector = NumEnemigos;
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
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Orange, TEXT("OBSERVER: Los enemigos han enfurecido"));
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

void ALevelBuilder::NotificarMuerteEnemigo()
{
    if (EnemigosVivosEnSector > 0)
    {
        EnemigosVivosEnSector--;
        if (EnemigosVivosEnSector == 0)
        {
            if (bJefeAparecido && !bJefeDerrotado)
            {
                bJefeDerrotado = true;
                SpawnNucleo();
            }
            else if (!bJefeAparecido)
            {
                SpawnNucleo();
                // La siguiente oleada se generará al recoger el núcleo (desde NaveJugador)
            }
        }
    }
}