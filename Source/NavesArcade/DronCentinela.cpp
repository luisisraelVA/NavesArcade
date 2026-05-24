#include "DronCentinela.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"
#include "Proyectil.h" 
#include "TimerManager.h" 

ADronCentinela::ADronCentinela()
{
	PrimaryActorTick.bCanEverTick = true;
	MallaDron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaDron"));
	RootComponent = MallaDron;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCono(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (MallaCono.Succeeded()) MallaDron->SetStaticMesh(MallaCono.Object);

	EsferaDeteccion = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaDeteccion"));
	EsferaDeteccion->SetupAttachment(RootComponent);
	// En tu constructor ADronCentinela::ADronCentinela()
	EsferaDeteccion->InitSphereRadius(8000.0f); // ¡Ahora ven a kilómetros!

	VelocidadPatrulla = 300.0f;
	IndicePuntoActual = 0;
	ObjetivoActual = nullptr;
}

void ADronCentinela::BeginPlay()
{
	Super::BeginPlay();
	EsferaDeteccion->OnComponentBeginOverlap.AddDynamic(this, &ADronCentinela::AlDetectarJugador);
	EsferaDeteccion->OnComponentEndOverlap.AddDynamic(this, &ADronCentinela::AlSalirJugador);
}

void ADronCentinela::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ObjetivoActual)
	{
		FVector DireccionHaciaJugador = (ObjetivoActual->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator RotacionSuave = FMath::RInterpTo(GetActorRotation(), DireccionHaciaJugador.Rotation(), DeltaTime, 5.0f);
		SetActorRotation(RotacionSuave);
	}
	else if (PuntosDeRuta.Num() > 0)
	{
		FVector Destino = PuntosDeRuta[IndicePuntoActual];
		FVector Direccion = (Destino - GetActorLocation()).GetSafeNormal();
		SetActorRotation(Direccion.Rotation());
		AddActorWorldOffset(Direccion * VelocidadPatrulla * DeltaTime, true);

		if (FVector::Dist(GetActorLocation(), Destino) < 50.0f)
		{
			IndicePuntoActual++;
			if (IndicePuntoActual >= PuntosDeRuta.Num()) IndicePuntoActual = 0;
		}
	}
}

void ADronCentinela::AlDetectarJugador(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<ANaveJugador>(OtherActor))
	{
		ObjetivoActual = OtherActor;
		// Iniciamos el bucle de preparación de ataque cada 1.5s
		GetWorldTimerManager().SetTimer(TimerCicloDisparo, this, &ADronCentinela::PrepararDisparo, 1.5f, true, 0.5f);
	}
}

void ADronCentinela::AlSalirJugador(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == ObjetivoActual)
	{
		ObjetivoActual = nullptr;
		GetWorldTimerManager().ClearTimer(TimerCicloDisparo);
		GetWorldTimerManager().ClearTimer(TimerEjecucionFisica);
		SetActorScale3D(FVector(1.0f, 1.0f, 1.0f)); // Restauramos tamaño por si huyó en medio de la carga
	}
}

void ADronCentinela::PrepararDisparo()
{
	if (ObjetivoActual)
	{
		// JUGABILIDAD (Telegrafiado): Aumentamos la escala para crear un efecto visual de "Carga de Energía"
		SetActorScale3D(FVector(1.3f, 1.3f, 1.3f));

		// 0.2 segundos después de crecer, dispara el láser real
		GetWorldTimerManager().SetTimer(TimerEjecucionFisica, this, &ADronCentinela::EjecutarDisparoLaser, 0.2f, false);
	}
}

void ADronCentinela::EjecutarDisparoLaser()
{
	if (ObjetivoActual)
	{
		// Volvemos al tamaño original como si liberara la presión
		SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

		FVector DireccionDisparo = GetActorForwardVector();
		FVector PuntoDeAparicion = GetActorLocation() + (DireccionDisparo * 200.0f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(this);

		GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), PuntoDeAparicion, DireccionDisparo.Rotation(), SpawnParams);
	}
}