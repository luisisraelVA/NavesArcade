// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveNodriza.generated.h"

UCLASS()
class NAVESARCADE_API ANaveNodriza : public APawn
{
	GENERATED_BODY()

public:
	ANaveNodriza();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UStaticMeshComponent* MallaPrincipal;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UStaticMeshComponent* AlaIzquierda;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UStaticMeshComponent* AlaDerecha;

	UPROPERTY(EditAnywhere, Category = "Combate")
	float VidaJefe;

	UPROPERTY(EditAnywhere, Category = "Combate")
	float DanioColision;

	UFUNCTION()
	void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void Tick(float DeltaTime) override;

	void RecibirDanio(float Cantidad);
};