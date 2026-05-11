// Fill out your copyright notice in the Description page of Project Settings.

#include "NaveNodriza.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"
#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"

ANaveNodriza::ANaveNodriza()
{
	PrimaryActorTick.bCanEverTick = true;

	MallaPrincipal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPrincipal"));
	RootComponent = MallaPrincipal;
	MallaPrincipal->SetNotifyRigidBodyCollision(true);

	AlaIzquierda = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlaIzquierda"));
	AlaIzquierda->SetupAttachment(RootComponent);

	AlaDerecha = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlaDerecha"));
	AlaDerecha->SetupAttachment(RootComponent);

	VidaJefe = 1000.0f;
	DanioColision = 50.0f;

	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANaveNodriza::BeginPlay()
{
	Super::BeginPlay();

	// Vincular el evento de colisión físicamente
	if (MallaPrincipal)
	{
		MallaPrincipal->OnComponentHit.AddDynamic(this, &ANaveNodriza::AlChocar);
	}
}

void ANaveNodriza::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotación lenta constante del jefe
	AddActorLocalRotation(FRotator(0.0f, 15.0f * DeltaTime, 0.0f));
}

void ANaveNodriza::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(ANaveJugador::StaticClass()))
	{
		ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
		if (Jugador)
		{
			Jugador->RecibirDano(DanioColision);
		}
	}
}

void ANaveNodriza::RecibirDanio(float Cantidad)
{
	VidaJefe -= Cantidad;
	if (VidaJefe <= 0.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("¡NAVE NODRIZA ELIMINADA!"));
		}
		Destroy();
	}
}