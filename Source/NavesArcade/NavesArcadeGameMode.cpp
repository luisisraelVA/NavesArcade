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
#include "PortalSalto.h" // Incluido para garantizar la limpieza nativa

ANavesArcadeGameMode::ANavesArcadeGameMode()
{
	DefaultPawnClass = ANaveJugador::StaticClass();
	HUDClass = ADodgerHUD::StaticClass();

	NivelActual = 1;
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

		// CORRECCIÓN: Detecta automáticamente en qué mapa físico físico estás jugando
		FString NombreMapa = GetWorld()->GetMapName();
		NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // Limpia prefijos internos de Unreal

		if (NombreMapa.Equals(TEXT("Nivel-02")))
		{
			NivelActual = 2;
		}
		else if (NombreMapa.Equals(TEXT("Nivel-03")))
		{
			NivelActual = 3;
		}
		else
		{
			NivelActual = 1; // Si estás en Nivel-01 o cualquier otro mapa base
		}

		CargarRecetaNivel(NivelActual);
	}
}

void ANavesArcadeGameMode::CargarRecetaNivel(int32 NumeroNivel)
{
	if (!InstanciaBuilder || !InstanciaDirector) return;

	// 1. Configuración de Fábricas Abstractas de IA según el Sector
	if (NumeroNivel <= 6)
	{
		InstanciaBuilder->SetFabrica(FabricaFaseUno);
	}
	else
	{
		InstanciaBuilder->SetFabrica(FabricaFaseFinal);
	}

	// 2. Inyección dinámica de la sub-clase de asteroide correspondiente
	if (NumeroNivel == 1)
	{
		InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
	}
	else if (NumeroNivel == 2)
	{
		InstanciaBuilder->SetClaseAsteroide(AAsteroideErratico::StaticClass());
	}
	else if (NumeroNivel == 3)
	{
		InstanciaBuilder->SetClaseAsteroide(AAsteroideExplosivo::StaticClass());
	}
	else
	{
		InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
	}

	// 3. El director ejecuta la receta matemática exacta de cantidades
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
	default:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("¡JUEGO TERMINADO!"));
		break;
	}
}

void ANavesArcadeGameMode::AvanzarSiguienteNivel()
{
	// Detectamos el mapa actual antes de realizar el salto físico de nivel
	FString NombreMapa = GetWorld()->GetMapName();
	NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (NombreMapa.Equals(TEXT("Nivel-01")))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("Nivel-02"));
	}
	else if (NombreMapa.Equals(TEXT("Nivel-02")))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("Nivel-03"));
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡FELICIDADES! COMPLETADO EL RECORRIDO"));
		}
		UGameplayStatics::OpenLevel(GetWorld(), FName("Nivel-01"));
	}
}

void ANavesArcadeGameMode::LimpiarMapa()
{
	TArray<AActor*> Asteroides;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAsteroideBase::StaticClass(), Asteroides);
	for (AActor* Asteroide : Asteroides)
	{
		if (Asteroide) Asteroide->Destroy();
	}

	TArray<AActor*> Energias;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANucleoEnergia::StaticClass(), Energias);
	for (AActor* Energia : Energias)
	{
		if (Energia) Energia->Destroy();
	}

	// C++ PURO: Limpieza de portales viejos para evitar duplicación fantasma
	TArray<AActor*> Portales;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortalSalto::StaticClass(), Portales);
	for (AActor* Portal : Portales)
	{
		if (Portal) Portal->Destroy();
	}
}