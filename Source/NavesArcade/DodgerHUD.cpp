// Fill out your copyright notice in the Description page of Project Settings.

#include "DodgerHUD.h"
#include "Engine/Canvas.h"
#include "NaveJugador.h"
#include "NaveFacade.h"
#include "UserInterfaceWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ADodgerHUD::ADodgerHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> FontObj(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	if (FontObj.Succeeded())
	{
		FuentePrincipal = FontObj.Object;
	}
}

void ADodgerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ClaseWidgetHUD)
	{
		MiWidget = CreateWidget<UUserInterfaceWidget>(GetWorld(), ClaseWidgetHUD);
		if (MiWidget)
		{
			MiWidget->AddToViewport();
		}
	}
}

void ADodgerHUD::DrawHUD()
{
	Super::DrawHUD();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ANaveJugador* MiNave = Cast<ANaveJugador>(PlayerPawn);

	if (MiNave)
	{
		UNaveFacade* Facade = MiNave->FindComponentByClass<UNaveFacade>();
		if (Facade)
		{
			// Extraemos los datos reales calculados dinámicamente
			float VidaReal = Facade->ObtenerVidaNave();
			float EnergiaReal = Facade->ObtenerEnergiaNave();

			// --- Sistema 1: Dibujo simple en Canvas ---
			FString TextoEnergia = FString::Printf(TEXT("SISTEMAS ACTIVOS - ENERGIA: %.0f / 60"), EnergiaReal);
			DrawText(TextoEnergia, FColor::Cyan, 50.f, 50.f, FuentePrincipal, 1.2f);

			// --- Sistema 2: Actualización del Widget de UMG en tiempo real ---
			if (MiWidget)
			{
				MiWidget->ActualizarHUD(VidaReal, EnergiaReal);
			}
		}
	}
}