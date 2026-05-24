#include "NaveJugador.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "InventoryComponent.h"
#include "NaveFacade.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Proyectil.h" 
#include "PortalSalto.h"
#include "LevelBuilder.h" // NUEVO: Para comunicarnos con el creador de niveles
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ANaveJugador::ANaveJugador()
{
	PrimaryActorTick.bCanEverTick = true;
	MallaNave = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNave"));
	RootComponent = MallaNave;
	MallaNave->SetEnableGravity(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModeloNaveAsset(TEXT("StaticMesh'/Game/Flying/Meshes/UFO.UFO'"));
	if (ModeloNaveAsset.Succeeded()) MallaNave->SetStaticMesh(ModeloNaveAsset.Object);

	BrazoCamara = CreateDefaultSubobject<USpringArmComponent>(TEXT("BrazoCamara"));
	BrazoCamara->SetupAttachment(RootComponent);
	BrazoCamara->TargetArmLength = 500.0f;
	BrazoCamara->bEnableCameraLag = true;
	BrazoCamara->CameraLagSpeed = 3.0f;
	BrazoCamara->bEnableCameraRotationLag = true;
	BrazoCamara->CameraRotationLagSpeed = 3.0f;

	CamaraNave = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraNave"));
	CamaraNave->SetupAttachment(BrazoCamara);

	Inventario = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventario"));
	FachadaNave = CreateDefaultSubobject<UNaveFacade>(TEXT("FachadaNave"));

	IntegridadEstructural = 100.0f;
	VelocidadMovimiento = 1000.0f;
	VelocidadRotacion = 85.0f;
	VidasActuales = 3;
	PuntuacionTotal = 0;
	MultiplicadorCombo = 1.0f;
	TiempoTemblorCamara = 0.0f;
	NucleosRecolectados = 0;

	// Aseguramos que siempre tengamos la clase del proyectil cargada
	ClaseProyectil = AProyectil::StaticClass();
}

void ANaveJugador::BeginPlay() { Super::BeginPlay(); }

void ANaveJugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TiempoTemblorCamara > 0.0f)
	{
		TiempoTemblorCamara -= DeltaTime;
		BrazoCamara->SocketOffset = FVector(FMath::RandRange(-20.f, 20.f), FMath::RandRange(-20.f, 20.f), FMath::RandRange(-20.f, 20.f));
	}
	else BrazoCamara->SocketOffset = FVector::ZeroVector;
}

void ANaveJugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoverAdelante", this, &ANaveJugador::MoverAdelante);
	PlayerInputComponent->BindAxis("RotarDerecha", this, &ANaveJugador::RotarDerecha);
	PlayerInputComponent->BindAxis("RotarArriba", this, &ANaveJugador::RotarArriba);
	PlayerInputComponent->BindAction("Disparar", IE_Pressed, this, &ANaveJugador::InicializarDisparo);
}

void ANaveJugador::InicializarDisparo()
{
	// Verificamos que la clase del láser esté cargada correctamente
	if (ClaseProyectil)
	{
		// 1. Alejamos el punto de aparición a 300 unidades para librar el fuselaje de la nave
		FVector PuntoSpawn = GetActorLocation() + (GetActorForwardVector() * 300.0f);

		FActorSpawnParameters Parametros;
		Parametros.Owner = this; // La nave es dueña del láser
		Parametros.Instigator = this; // Confirmamos que el jugador es quien apretó el gatillo

		// 2. ¡LA MAGIA!: Obligamos a Unreal a instanciar el láser ignorando las colisiones iniciales
		Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AProyectil>(ClaseProyectil, PuntoSpawn, GetActorRotation(), Parametros);
	}
}

void ANaveJugador::MoverAdelante(float Valor)
{
	if (Valor != 0.0f) AddActorWorldOffset(GetActorForwardVector() * Valor * VelocidadMovimiento * GetWorld()->GetDeltaSeconds(), true);
}

void ANaveJugador::RotarDerecha(float Valor)
{
	if (Valor != 0.0f) AddActorLocalRotation(FRotator(0.0f, Valor * VelocidadRotacion * GetWorld()->GetDeltaSeconds(), 0.0f));
}

void ANaveJugador::RotarArriba(float Valor)
{
	if (Valor != 0.0f) AddActorLocalRotation(FRotator(Valor * VelocidadRotacion * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f));
}

void ANaveJugador::RecibirDano(float CantidadDano)
{
	if (IntegridadEstructural <= 0.0f) return;
	IntegridadEstructural -= CantidadDano;
	MultiplicadorCombo = 1.0f;
	TiempoTemblorCamara = 0.5f;

	if (IntegridadEstructural <= 0.0f)
	{
		VidasActuales--;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		if (VidasActuales > 0)
		{
			FTimerHandle TimerRespawn;
			GetWorldTimerManager().SetTimer(TimerRespawn, this, &ANaveJugador::Reaparecer, 2.0f, false);
		}
		else
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC) DisableInput(PC);
			FTimerHandle TimerReinicio;
			GetWorldTimerManager().SetTimer(TimerReinicio, this, &ANaveJugador::ReiniciarNivel, 3.5f, false);
		}
	}
}

void ANaveJugador::Reaparecer()
{
	SetActorLocation(GetActorLocation() + FVector(-3000.0f, FMath::RandRange(-2000.0f, 2000.0f), 0.0f));
	SetActorRotation(FRotator::ZeroRotator);
	IntegridadEstructural = 100.0f;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ANaveJugador::RecolectarEnergia(float Cantidad)
{
	NucleosRecolectados++;
	EnergiaActual = (float)NucleosRecolectados;
	SumarPuntos(1000);

	if (NucleosRecolectados == 3)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, TEXT("¡PORTAL DETECTADO! DIRÍGETE A LAS COORDENADAS"));
		FVector UbicacionPortal = GetActorLocation() + (GetActorForwardVector() * 6000.0f);
		GetWorld()->SpawnActor<APortalSalto>(APortalSalto::StaticClass(), UbicacionPortal, FRotator::ZeroRotator);
	}
	else
	{
		// Si aún faltan núcleos, le decimos al Creador de Niveles que genere la siguiente base
		AActor* BuilderActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass());
		if (BuilderActor) Cast<ALevelBuilder>(BuilderActor)->GenerarFaseObjetivo();
	}
}

void ANaveJugador::SumarPuntos(int32 PuntosBase)
{
	PuntuacionTotal += (PuntosBase * MultiplicadorCombo);
	MultiplicadorCombo += 0.1f;
}

void ANaveJugador::ReiniciarNivel()
{
	FString NombreMapa = GetWorld()->GetMapName();
	NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NombreMapa));
}