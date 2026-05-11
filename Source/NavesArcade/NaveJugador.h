// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

class UInventoryComponent;

UCLASS()
class NAVESARCADE_API ANaveJugador : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANaveJugador();
	float EnergiaActual;

protected:

	virtual void BeginPlay() override;
	class UStaticMeshComponent* MallaNave;

	class USpringArmComponent* BrazoCamara;

	class UCameraComponent* CamaraNave;

	float IntegridadEstructural;

	float VelocidadMovimiento;

	float VelocidadRotacion;




	void RotarDerecha(float Valor); 
	void RotarArriba(float Valor);  


	void MoverAdelante(float Valor);

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void RecibirDano(float CantidadDano);


	void RecolectarEnergia(float Cantidad);


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventario;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UNaveFacade* FachadaNave;

public:

	FORCEINLINE class UInventoryComponent* GetInventario() const { return Inventario; }
};
