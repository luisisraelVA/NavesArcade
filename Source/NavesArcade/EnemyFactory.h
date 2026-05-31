#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyFactory.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    Sentry      UMETA(DisplayName = "Sentry"),
    Hunter      UMETA(DisplayName = "Hunter"),
    Boss        UMETA(DisplayName = "Boss"),
    Hybrid      UMETA(DisplayName = "Hybrid"),
    Suicide     UMETA(DisplayName = "Suicide"),
    Elite       UMETA(DisplayName = "Elite")      
};

UCLASS()
class NAVESARCADE_API UEnemyFactory : public UObject
{
    GENERATED_BODY()

public:
    static class AActor* SpawnEnemy(class UWorld* World, EEnemyType Type, FVector Location);
};