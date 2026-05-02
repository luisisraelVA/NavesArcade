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

protected:
	virtual void BeginPlay() override;

	class USphereComponent* EsferaColision;

	class UStaticMeshComponent* MallaAsteroide;

	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float DanoPorChoque;

public:
	virtual void Tick(float DeltaTime) override;
};