// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

class UInventoryComponent;
class UNaveFacade;
class AProyectil;

UCLASS()
class NAVESARCADE_API ANaveJugador : public APawn
{
	GENERATED_BODY()

public:
	ANaveJugador();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributos")
	float EnergiaActual;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class UStaticMeshComponent* MallaNave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
	class USpringArmComponent* BrazoCamara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
	class UCameraComponent* CamaraNave;

	UPROPERTY(EditAnywhere, Category = "Combate")
	TSubclassOf<AProyectil> ClaseProyectil;

	// Función puente nativa para evitar crash de inputs
	void InicializarDisparo();

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
	UInventoryComponent* Inventario;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	UNaveFacade* FachadaNave;

public:
	FORCEINLINE UInventoryComponent* GetInventario() const { return Inventario; }

	// Getter público para que la fachada extraiga la vida real
	FORCEINLINE float GetIntegridadEstructural() const { return IntegridadEstructural; }
};