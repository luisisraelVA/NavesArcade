#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DronCentinela.generated.h"

UCLASS()
class NAVESARCADE_API ADronCentinela : public AActor
{
	GENERATED_BODY()

public:
	ADronCentinela();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaDron;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class USphereComponent* EsferaDeteccion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA")
	TArray<FVector> PuntosDeRuta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA")
	float VelocidadPatrulla;

	int32 IndicePuntoActual;

	UPROPERTY()
	AActor* ObjetivoActual;

	FTimerHandle TimerCicloDisparo;
	FTimerHandle TimerEjecucionFisica; // NUEVO: Temporizador para la ventana de esquiva

	UFUNCTION()
	void AlDetectarJugador(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AlSalirJugador(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// NUEVO: Fase 1 (Aviso visual) y Fase 2 (Disparo real)
	void PrepararDisparo();
	void EjecutarDisparoLaser();

public:
	virtual void Tick(float DeltaTime) override;
};