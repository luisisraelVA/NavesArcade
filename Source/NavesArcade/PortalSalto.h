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
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class USphereComponent* EsferaColision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaSuperficie;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MallaSuperficieTrasera;

	bool bViajeIniciado;
};