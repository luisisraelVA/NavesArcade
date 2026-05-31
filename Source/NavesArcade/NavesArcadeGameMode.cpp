#include "NavesArcadeGameMode.h"
#include "NaveJugador.h"
#include "DodgerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "AsteroideErratico.h"
#include "AsteroideExplosivo.h"
#include "AsteroideFractal.h"
#include "NucleoEnergia.h"
#include "PortalSalto.h"
#include "LevelBuilder.h"
#include "LevelDirector.h"
#include "FaseUnoFab.h"
#include "FaseFinalFab.h"
#include "FaseAvanzadaFab.h"


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
            FString NumStr = NombreMapa.RightChop(6);
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
    if (NivelActual <= 2) NucleosRequeridos = 3;
    else if (NivelActual <= 4) NucleosRequeridos = 4;
    else if (NivelActual <= 6) NucleosRequeridos = 5;
    else if (NivelActual <= 9) NucleosRequeridos = 6;
    else NucleosRequeridos = 7;
}

void ANavesArcadeGameMode::CargarRecetaNivel(int32 NumeroNivel)
{
    if (!InstanciaBuilder || !InstanciaDirector) return;

    // Fábrica de enemigos según rango de niveles
    if (NumeroNivel <= 3)
        InstanciaBuilder->SetFabrica(FabricaFaseUno);
    else if (NumeroNivel <= 6)
        InstanciaBuilder->SetFabrica(FabricaFaseFinal);
    else
        InstanciaBuilder->SetFabrica(FabricaFaseAvanzada);

    // Tipo de asteroide
    if (NumeroNivel == 1 || NumeroNivel == 7)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
    else if (NumeroNivel == 2 || NumeroNivel == 8)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideErratico::StaticClass());
    else if (NumeroNivel == 3 || NumeroNivel == 9)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideExplosivo::StaticClass());
    else
        InstanciaBuilder->SetClaseAsteroide(AAsteroideFractal::StaticClass());

    switch (NumeroNivel)
    {
    case 1: InstanciaDirector->ConstruirNivel1(); break;
    case 2: InstanciaDirector->ConstruirNivel2(); break;
    case 3: InstanciaDirector->ConstruirNivel3(); break;
    case 4: InstanciaDirector->ConstruirNivel4(); break;
    case 5: InstanciaDirector->ConstruirNivel5(); break;
    case 6: InstanciaDirector->ConstruirNivel6(); break;
    case 7: InstanciaDirector->ConstruirNivel7(); break;
    case 8: InstanciaDirector->ConstruirNivel8(); break;
    case 9: InstanciaDirector->ConstruirNivel9(); break;
    case 10: InstanciaDirector->ConstruirNivel10(); break;
    case 11: InstanciaDirector->ConstruirNivel11(); break;
    case 12: InstanciaDirector->ConstruirNivel12(); break;
    default: InstanciaDirector->ConstruirNivel1(); break;
    }
}

void ANavesArcadeGameMode::AvanzarSiguienteNivel()
{
    int32 ProximoNivel = NivelActual + 1;
    if (ProximoNivel > 12) ProximoNivel = 1;   // Reinicia al 1 (o muestra final)
    FString NombreSiguienteNivel = FString::Printf(TEXT("Nivel-%02d"), ProximoNivel);
    FName NivelSiguiente = FName(*NombreSiguienteNivel);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Loading %s..."), *NombreSiguienteNivel));
    UGameplayStatics::OpenLevel(GetWorld(), NivelSiguiente);
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