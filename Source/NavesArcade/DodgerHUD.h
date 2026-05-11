// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DodgerHUD.generated.h"

UCLASS()
class NAVESARCADE_API ADodgerHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADodgerHUD();

	// Se ejecuta al iniciar el juego
	virtual void BeginPlay() override;

	// Se ejecuta cada frame (para el texto simple)
	virtual void DrawHUD() override;

protected:
	// Variable para asignar el WBP_PlayerHUD en el Editor
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserInterfaceWidget> ClaseWidgetHUD;

	// Puntero para el Widget
	UPROPERTY()
	class UUserInterfaceWidget* MiWidget;

	// Fuente para el sistema de dibujo antiguo
	UPROPERTY()
	class UFont* FuentePrincipal;
};