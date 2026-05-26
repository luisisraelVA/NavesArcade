#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

UCLASS()
class NAVESARCADE_API AEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyController();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "IA")
    class UBehaviorTree* ComportamientoEnemigo;
};