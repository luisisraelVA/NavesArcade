#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroideBase.generated.h"

UCLASS()
class NAVESARCADE_API AAsteroideBase : public AActor
{
	GENERATED_BODY()

public:
	AAsteroideBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class USphereComponent* EsferaColision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaAsteroide;

	UFUNCTION()
	virtual void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dano")
	float DanoPorChoque;

	float TiempoVisual;
	FVector EscalaOriginal;

	FTimerHandle TimerDespertar;
	void ReactivarTick();
};