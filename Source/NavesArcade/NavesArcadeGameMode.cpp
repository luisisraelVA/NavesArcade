#include "NavesArcadeGameMode.h"
#include "NaveJugador.h"
#include "DodgerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "AsteroideErratico.h"
#include "AsteroideExplosivo.h"
#include "NucleoEnergia.h"
#include "PortalSalto.h"
#include "LevelBuilder.h"
#include "LevelDirector.h"
#include "FaseUnoFab.h"
#include "FaseFinalFab.h"
#include "FaseAvanzadaFab.h"
#include "AsteroideFractal.h"

ANavesArcadeGameMode::ANavesArcadeGameMode()
{
    DefaultPawnClass = ANaveJugador::StaticClass();
    HUDClass = ADodgerHUD::StaticClass();
    NivelActual = 1;
    NucleosRequeridos = 3;
}

void ANavesArcadeGameMode::BeginPlay()
{
    Super::BeginPlay();

    FabricaFaseUno = NewObject<UFaseUnoFab>(this);
    FabricaFaseFinal = NewObject<UFaseFinalFab>(this);
    FabricaFaseAvanzada = NewObject<UFaseAvanzadaFab>(this);

    FVector PosicionBuilder = FVector(1500.0f, 0.0f, 0.0f);
    InstanciaBuilder = GetWorld()->SpawnActor<ALevelBuilder>(ALevelBuilder::StaticClass(), PosicionBuilder, FRotator::ZeroRotator);
    InstanciaDirector = GetWorld()->SpawnActor<ALevelDirector>(ALevelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    if (InstanciaBuilder && InstanciaDirector)
    {
        InstanciaDirector->SetBuilder(InstanciaBuilder);
        InstanciaDirector->SetDificultad(DificultadActual);

        FString NombreMapa = GetWorld()->GetMapName();
        NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

        if (NombreMapa.StartsWith("Nivel-"))
        {
            FString NumStr = NombreMapa.Right(2);
            NivelActual = FCString::Atoi(*NumStr);
        }
        else
        {
            NivelActual = 1;
        }

        ActualizarRequerimientoPorNivel();
        CargarRecetaNivel(NivelActual);
    }
}

void ANavesArcadeGameMode::ActualizarRequerimientoPorNivel()
{
    if (NivelActual <= 2)          NucleosRequeridos = 1;
    else if (NivelActual <= 4)     NucleosRequeridos = 1;
    else if (NivelActual <= 6)     NucleosRequeridos = 1;
    else if (NivelActual <= 9)     NucleosRequeridos = 1;
    else if (NivelActual == 12)    NucleosRequeridos = 1;
    else                           NucleosRequeridos = 1;
}

void ANavesArcadeGameMode::CargarRecetaNivel(int32 NumeroNivel)
{
    if (!InstanciaBuilder || !InstanciaDirector) return;

    if (NumeroNivel <= 3)
        InstanciaBuilder->SetFabrica(FabricaFaseUno);
    else if (NumeroNivel <= 6)
        InstanciaBuilder->SetFabrica(FabricaFaseFinal);
    else
        InstanciaBuilder->SetFabrica(FabricaFaseAvanzada);

    if (NumeroNivel == 1)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
    else if (NumeroNivel == 2)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideErratico::StaticClass());
    else if (NumeroNivel == 8 || NumeroNivel == 10 || NumeroNivel == 11)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideFractal::StaticClass());
    else if (NumeroNivel == 9 || NumeroNivel == 12)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideExplosivo::StaticClass());
    else
        InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());

    switch (NumeroNivel)
    {
    case 1:  InstanciaDirector->ConstruirNivel1();  break;
    case 2:  InstanciaDirector->ConstruirNivel2();  break;
    case 3:  InstanciaDirector->ConstruirNivel3();  break;
    case 4:  InstanciaDirector->ConstruirNivel4();  break;
    case 5:  InstanciaDirector->ConstruirNivel5();  break;
    case 6:  InstanciaDirector->ConstruirNivel6();  break;
    case 7:  InstanciaDirector->ConstruirNivel7();  break;
    case 8:  InstanciaDirector->ConstruirNivel8();  break;
    case 9:  InstanciaDirector->ConstruirNivel9();  break;
    case 10: InstanciaDirector->ConstruirNivel10(); break;
    case 11: InstanciaDirector->ConstruirNivel11(); break;
    case 12: InstanciaDirector->ConstruirNivel12(); break;
    default: InstanciaDirector->ConstruirNivel1();  break;
    }
}

void ANavesArcadeGameMode::AvanzarSiguienteNivel()
{
    int32 ProximoNivel = NivelActual + 1;
    if (ProximoNivel > NUMERO_TOTAL_NIVELES)
    {
        bJuegoCompletado = true;
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("¡VICTORIA! Has completado todos los niveles."));

        // Desactivar el control del jugador para que no pueda moverse
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
            PC->DisableInput(PC);

        return;
    }

    FString NombreSiguienteNivel = FString::Printf(TEXT("Nivel-%02d"), ProximoNivel);
    UGameplayStatics::OpenLevel(GetWorld(), FName(*NombreSiguienteNivel));
}

void ANavesArcadeGameMode::LimpiarMapa()
{
    TArray<AActor*> Asteroides;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAsteroideBase::StaticClass(), Asteroides);
    for (AActor* Asteroide : Asteroides) if (Asteroide) Asteroide->Destroy();

    TArray<AActor*> Energias;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANucleoEnergia::StaticClass(), Energias);
    for (AActor* Energia : Energias) if (Energia) Energia->Destroy();

    TArray<AActor*> Portales;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortalSalto::StaticClass(), Portales);
    for (AActor* Portal : Portales) if (Portal) Portal->Destroy();
}