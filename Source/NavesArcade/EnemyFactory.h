#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyFactory.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8 {
    Sentry, Suicide, Elite, Hunter, Hibrido, Boss 
};

UCLASS()
class NAVESARCADE_API UEnemyFactory : public UObject
{
    GENERATED_BODY()

public:
    static class AActor* SpawnEnemy(class UWorld* World, EEnemyType Type, FVector Location);
};