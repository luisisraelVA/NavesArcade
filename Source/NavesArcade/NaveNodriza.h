// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveNodriza.generated.h"

class UStaticMeshComponent;
class UPrimitiveComponent;
class AProyectil;

UCLASS()
class NAVESARCADE_API ANaveNodriza : public APawn
{
	GENERATED_BODY()

public:
	ANaveNodriza();

	virtual void Tick(float DeltaTime) override;

	void RecibirDanio(float Cantidad);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* MallaPrincipal;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* AlaIzquierda;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* AlaDerecha;

	UPROPERTY(EditAnywhere, Category = "Combate")
	float VidaJefe;

	UPROPERTY(EditAnywhere, Category = "Combate")
	float DanioColision;

	UPROPERTY(EditAnywhere, Category = "Combate")
	float VelocidadRotacion;

	UPROPERTY(EditAnywhere, Category = "Combate")
	float TiempoEntreAtaques;

	float TiempoAtaqueActual;

	UFUNCTION()
	void AlChocar(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	void EjecutarAtaque();
};