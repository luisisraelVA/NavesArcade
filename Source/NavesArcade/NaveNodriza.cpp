// Fill out your copyright notice in the Description page of Project Settings.

#include "NaveNodriza.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"
#include "EnemyController.h"
#include "Engine/Engine.h"

ANaveNodriza::ANaveNodriza()
{
	PrimaryActorTick.bCanEverTick = true;

	MallaPrincipal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPrincipal"));
	RootComponent = MallaPrincipal;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaEsfera(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MallaEsfera.Succeeded())
	{
		MallaPrincipal->SetStaticMesh(MallaEsfera.Object);
	}

	MallaPrincipal->SetNotifyRigidBodyCollision(true);
	MallaPrincipal->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	AlaIzquierda = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlaIzquierda"));
	AlaIzquierda->SetupAttachment(RootComponent);

	AlaDerecha = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlaDerecha"));
	AlaDerecha->SetupAttachment(RootComponent);

	VidaJefe = 1000.0f;
	DanioColision = 50.0f;
	VelocidadRotacion = 15.0f;
	TiempoEntreAtaques = 2.5f;
	TiempoAtaqueActual = 0.0f;

	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANaveNodriza::BeginPlay()
{
	Super::BeginPlay();

	if (MallaPrincipal)
	{
		MallaPrincipal->OnComponentHit.AddDynamic(this, &ANaveNodriza::AlChocar);
	}
}

void ANaveNodriza::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, VelocidadRotacion * DeltaTime, 0.0f));

	TiempoAtaqueActual += DeltaTime;

	if (TiempoAtaqueActual >= TiempoEntreAtaques)
	{
		EjecutarAtaque();
		TiempoAtaqueActual = 0.0f;
	}
}

void ANaveNodriza::AlChocar(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);

	if (Jugador)
	{
		Jugador->RecibirDano(DanioColision);
	}
}

void ANaveNodriza::EjecutarAtaque()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Purple,
			TEXT("Nave Nodriza ejecuta ataque final")
		);
	}
}

void ANaveNodriza::RecibirDanio(float Cantidad)
{
	VidaJefe -= Cantidad;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Vida Jefe: %.0f"), VidaJefe)
		);
	}

	if (VidaJefe <= 0.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Yellow,
				TEXT("¡NAVE NODRIZA ELIMINADA!")
			);
		}

		Destroy();
	}
}