// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

// Forward declarations para evitar errores de compilación
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

	// --- COMPONENTES VISUALES ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class UStaticMeshComponent* MallaNave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
	class USpringArmComponent* BrazoCamara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
	class UCameraComponent* CamaraNave;

	// --- COMBATE ---
	UPROPERTY(EditAnywhere, Category = "Combate")
	TSubclassOf<AProyectil> ClaseProyectil;

	void Disparar();

	// --- ATRIBUTOS ---
	float IntegridadEstructural;
	float VelocidadMovimiento;
	float VelocidadRotacion;

	// --- MÉTODOS DE MOVIMIENTO ---
	void RotarDerecha(float Valor);
	void RotarArriba(float Valor);
	void MoverAdelante(float Valor);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RecibirDano(float CantidadDano);
	void RecolectarEnergia(float Cantidad);

private:
	// --- COMPONENTES DE SISTEMA ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* Inventario;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	UNaveFacade* FachadaNave;

public:
	// Getter para el inventario
	FORCEINLINE UInventoryComponent* GetInventario() const { return Inventario; }
};