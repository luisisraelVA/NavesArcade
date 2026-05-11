// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaceWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUserInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Inicialización si es necesaria
}

void UUserInterfaceWidget::ActualizarHUD(float VidaActual, float EnergiaActual)
{
	// Actualizar la barra de vida (valor entre 0.0 y 1.0)
	if (BarraVida)
	{
		BarraVida->SetPercent(VidaActual / 100.0f);
	}

	// Actualizar el texto de energía
	if (TextoEnergia)
	{
		FString StringEnergia = FString::Printf(TEXT("Energía: %.0f / 60"), EnergiaActual);
		TextoEnergia->SetText(FText::FromString(StringEnergia));
	}
}