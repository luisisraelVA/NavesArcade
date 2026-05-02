// Fill out your copyright notice in the Description page of Project Settings.


#include "DronCentinela.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"

// Sets default values
ADronCentinela::ADronCentinela()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MallaDron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaDron"));
	RootComponent = MallaDron;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCono(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (MallaCono.Succeeded())
	{
		MallaDron->SetStaticMesh(MallaCono.Object);
	}

	EsferaDeteccion = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaDeteccion"));
	EsferaDeteccion->SetupAttachment(RootComponent);
	EsferaDeteccion->InitSphereRadius(800.0f);

	VelocidadPatrulla = 300.0f;
	IndicePuntoActual = 0;
}


void ADronCentinela::BeginPlay()
{
	Super::BeginPlay();
	EsferaDeteccion->OnComponentBeginOverlap.AddDynamic(this, &ADronCentinela::AlDetectarJugador);
}


void ADronCentinela::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PuntosDeRuta.Num() > 0)
	{
		FVector Destino = PuntosDeRuta[IndicePuntoActual];

		FVector Direccion = (Destino - GetActorLocation()).GetSafeNormal();
		AddActorWorldOffset(Direccion * VelocidadPatrulla * DeltaTime, true);

		if (FVector::Dist(GetActorLocation(), Destino) < 50.0f)
		{
			IndicePuntoActual++;

			
			if (IndicePuntoActual >= PuntosDeRuta.Num())
			{
				IndicePuntoActual = 0;
			}
		}
	}
}


void ADronCentinela::AlDetectarJugador(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<ANaveJugador>(OtherActor))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("¡ALERTA CENTINELA!"));
		}
	}
}
