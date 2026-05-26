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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class USphereComponent* EsferaColision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaAsteroide;

    UFUNCTION()
    void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Daño")
    float DanoPorChoque;

    float TiempoVisual;
    FVector EscalaOriginal;

public:
    virtual void Tick(float DeltaTime) override;
};