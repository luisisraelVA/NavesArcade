#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemigoBase.generated.h"

UCLASS()
class NAVESARCADE_API AEnemigoBase : public APawn
{
    GENERATED_BODY()

public:
    AEnemigoBase();
    virtual void RecibirDano(float Cantidad);

protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Colisión")
    class USphereComponent* EsferaColision;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float Escudo;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float Salud;
};