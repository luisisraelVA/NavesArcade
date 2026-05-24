#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveNodriza.generated.h"

UCLASS()
class NAVESARCADE_API ANaveNodriza : public APawn
{
	GENERATED_BODY()

public:
	ANaveNodriza();
	virtual void Tick(float DeltaTime) override;

	// Función expuesta para recibir los impactos del jugador
	void RecibirDano(float Dano);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* MallaJefe;

	class UParticleSystem* EfectoMuerteMasiva;

	float VidaActual;
	float VidaMaxima;
	bool bFaseDos;
	bool bMuerto;

	FTimerHandle TimerDisparoAbanico;
	FTimerHandle TimerInvocacion;
	FTimerHandle TimerHitFlash;
	FTimerHandle TimerMuerteCinematica;

	// Habilidades del Jefe
	void DispararAbanico();
	void InvocarKamikaze();

	// Game Feel
	void RestaurarMaterial();
	void FinalizarMuerteCinematica();
};