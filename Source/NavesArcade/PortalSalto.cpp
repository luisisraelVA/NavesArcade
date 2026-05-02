// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalSalto.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h" 


APortalSalto::APortalSalto()
{
	PrimaryActorTick.bCanEverTick = false;
	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(200.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MallaPortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPortal"));
	MallaPortal->SetupAttachment(RootComponent);
	MallaPortal->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCilindro(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (MallaCilindro.Succeeded())
	{
		MallaPortal->SetStaticMesh(MallaCilindro.Object);
	}

	
	EnergiaRequerida = 60.0f;
}


void APortalSalto::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &APortalSalto::AlSuperponerse);
}


void APortalSalto::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalSalto::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ANaveJugador* NaveChocada = Cast<ANaveJugador>(OtherActor);

		if (NaveChocada)
		{
			
			if (NaveChocada->EnergiaActual >= EnergiaRequerida)
			{
				if (GEngine)
				{

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("¡PORTAL ACTIVADO! Nivel completado."));
				}

			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Energía insuficiente para el salto."));
				}
			}
		}
	}
}
