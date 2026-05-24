#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelFab.h"
#include "LevelBuilder.generated.h"

UCLASS()
class NAVESARCADE_API ALevelBuilder : public AActor
{
	GENERATED_BODY()

public:
	ALevelBuilder();
	virtual void BeginPlay() override;

	void SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica);
	void SetClaseAsteroide(TSubclassOf<class AAsteroideBase> NuevaClase);

	void SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide);

	// NUEVO: Función que instanciará una base a la vez
	void GenerarFaseObjetivo();

protected:
	UFUNCTION()
	void GeneracionContinua();

private:
	TScriptInterface<ILevelFab> FabricaDeFase;
	TSubclassOf<class AAsteroideBase> ClaseAsteroide;
	TSubclassOf<class ANucleoEnergia> ClaseEnergia;

	FTimerHandle TimerMundoAbierto;

	int32 DensidadAsteroides;
	float VelocidadGlobal;
	int32 FaseActualMision; // Guarda en qué núcleo vamos
};