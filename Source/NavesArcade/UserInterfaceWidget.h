// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserInterfaceWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAVESARCADE_API UUserInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Se ejecuta cuando el widget se crea (como el BeginPlay)
	virtual void NativeConstruct() override;

	// Función para actualizar los datos desde fuera
	void ActualizarHUD(float VidaActual, float EnergiaActual);

protected:
	// Vincular estas variables con elementos visuales del mismo nombre en el Blueprint
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BarraVida;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextoEnergia;
};