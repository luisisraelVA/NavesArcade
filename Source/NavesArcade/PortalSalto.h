#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalSalto.generated.h"

UCLASS()
class NAVESARCADE_API APortalSalto : public AActor
{
	GENERATED_BODY()

public:
	APortalSalto();

protected:
	virtual void BeginPlay() override;

	// FIX: UPROPERTY evita punteros nulos en runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class USphereComponent* EsferaColision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaPortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requisitos")
	float EnergiaRequerida;

	// FIX: Registrar la funcion en el sistema de reflexion de Unreal
	UFUNCTION()
	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
};