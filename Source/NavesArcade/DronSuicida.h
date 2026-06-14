#pragma once
#include "CoreMinimal.h"
#include "EnemigoBase.h"
#include "DronSuicida.generated.h"

UCLASS()
class NAVESARCADE_API ADronSuicida : public AEnemigoBase
{
    GENERATED_BODY()
public:
    ADronSuicida();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* Malla;

    UPROPERTY(EditAnywhere, Category = "AI")
    float VelocidadCarga = 700.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DistanciaDeteccion = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Damage")
    float DanoExplosion = 50.0f;

    UPROPERTY()
    class APawn* Objetivo;

    bool bActivado = false;

    UPROPERTY()
    class USoundBase* SonidoImpactoDron;

    UFUNCTION()
    void AlImpactar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};