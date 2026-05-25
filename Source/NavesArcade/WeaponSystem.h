#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Proyectil.h" // <--- ¡AÑADE ESTO AQUÍ!
#include "WeaponSystem.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVESARCADE_API UWeaponSystem : public UActorComponent
{
	GENERATED_BODY()
	// ... el resto de tu código igual ...

public:
	UWeaponSystem();

protected:
	virtual void BeginPlay() override;

	// ¡AQUÍ RESTAURAMOS LA VARIABLE DEL PROYECTIL!
	UPROPERTY(EditAnywhere, Category = "Armamento")
	TSubclassOf<class AProyectil> ClaseProyectil;

	// Configuración de cadencia de tiro
	UPROPERTY(EditAnywhere, Category = "Armamento")
	float CadenciaDisparo;

	bool bPuedeDisparar;
	FTimerHandle TimerHandle_Cadencia;

	// Reinicia el estado para permitir otro disparo
	void ResetearDisparo();

public:
	// Método principal para ejecutar el ataque
	void Disparar(FVector Ubicacion, FRotator Rotacion);

	// Getters y Setters para Power-ups (POO: Encapsulamiento)
	void SetCadenciaDisparo(float NuevaCadencia) { CadenciaDisparo = NuevaCadencia; }
};