// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgerHUD.h"
#include "Engine/Canvas.h"
#include "NaveJugador.h"
#include "NaveFacade.h"
#include "UserInterfaceWidget.h" // <--- IMPORTANTE: Asegúrate de que este archivo existe
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ADodgerHUD::ADodgerHUD()
{
	// Buscamos una fuente básica del motor
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
			// --- Sistema 1: Dibujo simple (Cian) ---
			FString TextoEnergia = FString::Printf(TEXT("SISTEMAS ACTIVOS - ENERGIA: %.0f / 60"), 20.0f);
			DrawText(TextoEnergia, FColor::Cyan, 50.f, 50.f, FuentePrincipal, 1.2f);

			// --- Sistema 2: Actualizar el Widget (Barra de Vida) ---
			if (MiWidget)
			{
				// Supongamos que tu Fachada tiene GetVida() y GetEnergia()
				// Si no los tiene, usa valores de prueba para ver si se mueve
				MiWidget->ActualizarHUD(80.0f, 20.0f);
			}
		}
	}
}