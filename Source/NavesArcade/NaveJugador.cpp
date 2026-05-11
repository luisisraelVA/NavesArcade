// Fill out your copyright notice in the Description page of Project Settings.


#include "NaveJugador.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "InventoryComponent.h"
#include "NaveFacade.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Proyectil.h" // Importante incluir el header del nuevo proyectil

ANaveJugador::ANaveJugador()
{
	PrimaryActorTick.bCanEverTick = true;
	MallaNave = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNave"));
	RootComponent = MallaNave;
	MallaNave->SetEnableGravity(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModeloNaveAsset(TEXT("StaticMesh'/Game/Flying/Meshes/UFO.UFO'"));
	if (ModeloNaveAsset.Succeeded())
	{
		MallaNave->SetStaticMesh(ModeloNaveAsset.Object);
	}

	BrazoCamara = CreateDefaultSubobject<USpringArmComponent>(TEXT("BrazoCamara"));
	BrazoCamara->SetupAttachment(RootComponent);
	BrazoCamara->TargetArmLength = 500.0f;

	CamaraNave = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraNave"));
	CamaraNave->SetupAttachment(BrazoCamara, USpringArmComponent::SocketName);

	IntegridadEstructural = 100.0f;
	VelocidadMovimiento = 1000.0f;
	VelocidadRotacion = 100.0f;
	EnergiaActual = 0.0f;

	Inventario = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventario"));
	FachadaNave = CreateDefaultSubobject<UNaveFacade>(TEXT("FachadaNave"));
}

void ANaveJugador::BeginPlay()
{
	Super::BeginPlay();
}

void ANaveJugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANaveJugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoverAdelante", this, &ANaveJugador::MoverAdelante);
	PlayerInputComponent->BindAxis("RotarDerecha", this, &ANaveJugador::RotarDerecha);
	PlayerInputComponent->BindAxis("RotarArriba", this, &ANaveJugador::RotarArriba);

	// Vinculamos la acción de disparar (Debes crear "Disparar" en Project Settings -> Input)
	PlayerInputComponent->BindAction("Disparar", IE_Pressed, this, &ANaveJugador::Disparar);
}

void ANaveJugador::MoverAdelante(float Valor)
{
	if (Valor != 0.0f)
	{
		FVector NuevaPosicion = GetActorForwardVector() * Valor * VelocidadMovimiento * GetWorld()->GetDeltaSeconds();
		AddActorWorldOffset(NuevaPosicion, true);
	}
}

void ANaveJugador::RotarDerecha(float Valor)
{
	if (Valor != 0.0f)
	{
		FRotator NuevaRotacion = FRotator(0.0f, Valor * VelocidadRotacion * GetWorld()->GetDeltaSeconds(), 0.0f);
		AddActorLocalRotation(NuevaRotacion);
	}
}

void ANaveJugador::RotarArriba(float Valor)
{
	if (Valor != 0.0f)
	{
		FRotator NuevaRotacion = FRotator(Valor * VelocidadRotacion * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f);
		AddActorLocalRotation(NuevaRotacion);
	}
}

void ANaveJugador::Disparar()
{
	if (ClaseProyectil)
	{
		// Spawn un poco delante de la nave para evitar que choque con ella misma
		FVector UbicacionSpawn = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator RotacionSpawn = GetActorRotation();

		FActorSpawnParameters ParametrosSpawn;
		ParametrosSpawn.Owner = this;
		ParametrosSpawn.Instigator = GetInstigator();

		GetWorld()->SpawnActor<AProyectil>(ClaseProyectil, UbicacionSpawn, RotacionSpawn, ParametrosSpawn);
	}
}

void ANaveJugador::RecibirDano(float CantidadDano)
{
	IntegridadEstructural -= CantidadDano;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("¡Impacto! Vida actual: %f"), IntegridadEstructural));
	}

	if (IntegridadEstructural <= 0.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡NAVE DESTRUIDA!"));
		}
		Destroy();
	}
}

void ANaveJugador::RecolectarEnergia(float Cantidad)
{
	EnergiaActual += Cantidad;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("¡Núcleo recogido! Energía total: %f"), EnergiaActual));
	}
}