// Fill out your copyright notice in the Description page of Project Settings.


#include "NaveAcechadora.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyController.h" 

ANaveAcechadora::ANaveAcechadora()
{
	PrimaryActorTick.bCanEverTick = true;

	MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
	RootComponent = MallaEnemigo;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshAsset.Succeeded())
	{
		MallaEnemigo->SetStaticMesh(MeshAsset.Object);
	}

	VelocidadPersecucion = 400.0f; // Velocidad para naves acechadoras

	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANaveAcechadora::BeginPlay()
{
	Super::BeginPlay();
	TargetJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ANaveAcechadora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetJugador)
	{
		FVector Direccion = (TargetJugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		// Movimiento de persecución activa
		AddActorWorldOffset(Direccion * VelocidadPersecucion * DeltaTime, true);
		SetActorRotation(Direccion.Rotation());
	}
}
