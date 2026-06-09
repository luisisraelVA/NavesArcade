#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Proyectil.generated.h"

UCLASS()
class NAVESARCADE_API AProyectil : public AActor
{
	GENERATED_BODY()

public:
	AProyectil();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Colision")
	class USphereComponent* EsferaColision;

	// --- VOLVEMOS A USAR STATIC MESH PARA LA VISUAL ---
	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UStaticMeshComponent* MallaProyectil;

	UPROPERTY(VisibleAnywhere, Category = "Movimiento")
	class UProjectileMovementComponent* MovimientoProyectil;

	// Este se mantiene para la explosioncita al impactar
	class UParticleSystem* EfectoExplosion;

	UFUNCTION()
	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};