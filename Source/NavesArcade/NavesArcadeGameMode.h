// Copyright Epic Games, Inc. All Rights Reserved.

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelBuilder.h"
#include "LevelDirector.h"
#include "FaseUnoFab.h"
#include "FaseFinalFab.h"
#include "NavesArcadeGameMode.generated.h"

UCLASS(MinimalAPI)
class ANavesArcadeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANavesArcadeGameMode();

	// Función para avanzar de nivel (la llamará el portal de salto)
	void AvanzarSiguienteNivel();

protected:
	virtual void BeginPlay() override;

private:
	// Punteros para guardar tus patrones de diseño en memoria
	UPROPERTY()
	ALevelBuilder* InstanciaBuilder;

	UPROPERTY()
	ALevelDirector* InstanciaDirector;

	UPROPERTY()
	UFaseUnoFab* FabricaFaseUno;

	UPROPERTY()
	UFaseFinalFab* FabricaFaseFinal;

	// Variable para saber en qué nivel vamos
	int32 NivelActual;

	// Función interna para cargar la receta exacta del director
	void CargarRecetaNivel(int32 NumeroNivel);

	// Función para limpiar los asteroides viejos antes de armar el nuevo nivel
	void LimpiarMapa();
};
