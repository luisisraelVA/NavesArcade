// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalSalto.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h" 
#include "NaveFacade.h" // Incluimos el Facade para manejar la energía limpiamente
#include "NavesArcadeGameMode.h"
#include "Kismet/GameplayStatics.h"

APortalSalto::APortalSalto()
{
	PrimaryActorTick.bCanEverTick = false;

	// Componente de colisión por esfera
	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(400.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MallaPortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPortal"));
	MallaPortal->SetupAttachment(RootComponent);
	MallaPortal->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Usamos el cilindro básico nativo del motor que NUNCA falla al abrir Unreal
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCilindroNativo(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));

	if (MallaCilindroNativo.Succeeded())
	{
		MallaPortal->SetStaticMesh(MallaCilindroNativo.Object);

		// Lo aplanamos y ensanchamos por código para que parezca un anillo o compuerta estelar
		MallaPortal->SetRelativeScale3D(FVector(6.0f, 6.0f, 0.1f));
	}

	EnergiaRequerida = 60.0f;
}

void APortalSalto::BeginPlay()
{
	Super::BeginPlay();

	if (EsferaColision)
	{
		EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &APortalSalto::AlSuperponerse);
	}
}

void APortalSalto::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortalSalto::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// 1. Verificamos si el objeto que entró al portal es la nave del jugador
		ANaveJugador* NaveChocada = Cast<ANaveJugador>(OtherActor);
		if (NaveChocada)
		{
			// 2. CORRECCIÓN DEFINITIVA: Le pedimos al Facade de la nave que valide la energía por nosotros
			UNaveFacade* Facade = NaveChocada->FindComponentByClass<UNaveFacade>();

			if (Facade && Facade->PuedeSaltarDeNivel())
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("¡PORTAL ACTIVADO! Saltando de nivel físico..."));
				}

				// 3. Obtenemos el GameMode de C++ que maneja el OpenLevel
				ANavesArcadeGameMode* GameMode = Cast<ANavesArcadeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GameMode)
				{
					// 4. Le ordenamos abrir el siguiente mapa (Nivel-02 o Nivel-03)
					GameMode->AvanzarSiguienteNivel();
				}
			}
			else
			{
				// Mensaje de advertencia si la nave no tiene suficiente combustible de cristales
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Energía insuficiente. Requiere 60 puntos."));
				}
			}
		}
	}
}