// Copyright Epic Games, Inc. All Rights Reserved.

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

    FVector PosicionBuilder = FVector(1500.0f, 0.0f, 0.0f);
    InstanciaBuilder = GetWorld()->SpawnActor<ALevelBuilder>(ALevelBuilder::StaticClass(), PosicionBuilder, FRotator::ZeroRotator);
    InstanciaDirector = GetWorld()->SpawnActor<ALevelDirector>(ALevelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    if (InstanciaBuilder && InstanciaDirector)
    {
        InstanciaDirector->SetBuilder(InstanciaBuilder);
        InstanciaDirector->SetDificultad(DificultadActual);

        // Obtener el número del nivel por el nombre del mapa
        FString NombreMapa = GetWorld()->GetMapName();
        NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        if (NombreMapa.Equals(TEXT("Nivel-01"))) NivelActual = 1;
        else if (NombreMapa.Equals(TEXT("Nivel-02"))) NivelActual = 2;
        else if (NombreMapa.Equals(TEXT("Nivel-03"))) NivelActual = 3;
        else if (NombreMapa.Equals(TEXT("Nivel-04"))) NivelActual = 4;
        else if (NombreMapa.Equals(TEXT("Nivel-05"))) NivelActual = 5;
        else if (NombreMapa.Equals(TEXT("Nivel-06"))) NivelActual = 6;
        else NivelActual = 1;

        ActualizarRequerimientoPorNivel();
        CargarRecetaNivel(NivelActual);
    }
}

void ANavesArcadeGameMode::ActualizarRequerimientoPorNivel()
{
    if (NivelActual <= 2) NucleosRequeridos = 3;
    else if (NivelActual <= 4) NucleosRequeridos = 4;
    else NucleosRequeridos = 5;
}

void ANavesArcadeGameMode::CargarRecetaNivel(int32 NumeroNivel)
{
    if (!InstanciaBuilder || !InstanciaDirector) return;

    // Fábricas: niveles 1-3 FaseUno, 4-6 FaseFinal
    if (NumeroNivel <= 3)
        InstanciaBuilder->SetFabrica(FabricaFaseUno);
    else
        InstanciaBuilder->SetFabrica(FabricaFaseFinal);

    // Tipos de asteroides
    if (NumeroNivel == 1)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
    else if (NumeroNivel == 2)
        InstanciaBuilder->SetClaseAsteroide(AAsteroideErratico::StaticClass());
    else
        InstanciaBuilder->SetClaseAsteroide(AAsteroideExplosivo::StaticClass());

    // Construcción según nivel
    switch (NumeroNivel)
    {
    case 1: InstanciaDirector->ConstruirNivel1(); break;
    case 2: InstanciaDirector->ConstruirNivel2(); break;
    case 3: InstanciaDirector->ConstruirNivel3(); break;
    case 4: InstanciaDirector->ConstruirNivel4(); break;
    case 5: InstanciaDirector->ConstruirNivel5(); break;
    case 6: InstanciaDirector->ConstruirNivel6(); break;
    default: InstanciaDirector->ConstruirNivel1(); break;
    }
}

void ANavesArcadeGameMode::AvanzarSiguienteNivel()
{
    FString NombreMapa = GetWorld()->GetMapName();
    NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    FName NivelSiguiente;
    if (NombreMapa.Equals(TEXT("Nivel-01")))       NivelSiguiente = "Nivel-02";
    else if (NombreMapa.Equals(TEXT("Nivel-02"))) NivelSiguiente = "Nivel-03";
    else if (NombreMapa.Equals(TEXT("Nivel-03"))) NivelSiguiente = "Nivel-04";
    else if (NombreMapa.Equals(TEXT("Nivel-04"))) NivelSiguiente = "Nivel-05";
    else if (NombreMapa.Equals(TEXT("Nivel-05"))) NivelSiguiente = "Nivel-06";
    else NivelSiguiente = "Nivel-01";

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Cargando %s..."), *NivelSiguiente.ToString()));

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