#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NucleoEnergia.generated.h"

UCLASS()
class NAVESARCADE_API ANucleoEnergia : public AActor
{
	GENERATED_BODY()

public:
	ANucleoEnergia();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaNucleo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class USphereComponent* EsferaColision;

	UFUNCTION()
	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
};