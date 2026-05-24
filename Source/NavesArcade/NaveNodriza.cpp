#include "NaveNodriza.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Proyectil.h"
#include "NaveAcechadora.h"

ANaveNodriza::ANaveNodriza()
{
	PrimaryActorTick.bCanEverTick = true;

	MallaJefe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaJefe"));
	RootComponent = MallaJefe;

	// Usamos un cubo gigante como base visual del jefe
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MeshAsset.Succeeded()) MallaJefe->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticulaAsset.Succeeded()) EfectoMuerteMasiva = ParticulaAsset.Object;

	// Escala masiva (5 veces el tamaño normal)
	SetActorScale3D(FVector(5.0f, 5.0f, 5.0f));

	VidaMaxima = 500.0f; // 20 disparos del jugador para derribarlo
	VidaActual = VidaMaxima;
	bFaseDos = false;
	bMuerto = false;
}

void ANaveNodriza::BeginPlay()
{
	Super::BeginPlay();

	// Inicia su ataque principal: Un disparo múltiple cada 2 segundos
	GetWorldTimerManager().SetTimer(TimerDisparoAbanico, this, &ANaveNodriza::DispararAbanico, 2.0f, true, 1.0f);
}

void ANaveNodriza::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bMuerto) return;

	// Movimiento imponente: Flotación en el eje Z mediante Onda Senoidal + Avance ultra lento
	float Levitacion = FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f) * 100.0f * DeltaTime;
	AddActorWorldOffset(FVector(-100.0f * DeltaTime, 0.0f, Levitacion), true);

	// LÓGICA DE FASE 2: Si tiene menos de la mitad de vida, entra en pánico e invoca minions
	if (VidaActual <= (VidaMaxima / 2.0f) && !bFaseDos)
	{
		bFaseDos = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡ADVERTENCIA: PROTOCOLO DE DEFENSA NODRIZA ACTIVADO!"));

		// Empieza a escupir Naves Acechadoras cada 4 segundos
		GetWorldTimerManager().SetTimer(TimerInvocacion, this, &ANaveNodriza::InvocarKamikaze, 4.0f, true);
	}
}

void ANaveNodriza::DispararAbanico()
{
	if (bMuerto) return;

	// SPREAD SHOT: 3 Proyectiles simultáneos en abanico (-15, 0, y 15 grados)
	TArray<float> Angulos = { -15.0f, 0.0f, 15.0f };
	FVector Frente = GetActorForwardVector();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;

	for (float Angulo : Angulos)
	{
		FRotator RotacionFuego = Frente.Rotation();
		RotacionFuego.Yaw += Angulo; // Desviamos la trayectoria
		FVector PuntoAparicion = GetActorLocation() + (RotacionFuego.Vector() * 500.0f);

		GetWorld()->SpawnActor<AProyectil>(AProyectil::StaticClass(), PuntoAparicion, RotacionFuego, SpawnParams);
	}
}

void ANaveNodriza::InvocarKamikaze()
{
	if (bMuerto) return;
	FVector PuntoSpawn = GetActorLocation() + FVector(0.0f, 0.0f, -400.0f); // Salen por debajo de la nave
	GetWorld()->SpawnActor<ANaveAcechadora>(ANaveAcechadora::StaticClass(), PuntoSpawn, FRotator::ZeroRotator);
}

void ANaveNodriza::RecibirDano(float Dano)
{
	if (bMuerto) return;
	VidaActual -= Dano;

	// HIT FLASHING EN C++: Encogemos la nave ligerísimamente por 0.1s para simular compresión por impacto
	SetActorScale3D(FVector(4.8f, 4.8f, 4.8f));
	GetWorldTimerManager().SetTimer(TimerHitFlash, this, &ANaveNodriza::RestaurarMaterial, 0.1f, false);

	if (VidaActual <= 0.0f)
	{
		bMuerto = true;
		GetWorldTimerManager().ClearAllTimersForObject(this); // Apagamos todas sus armas

		// HIT STOP: Cámara lenta dramática global
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);

		if (EfectoMuerteMasiva) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoMuerteMasiva, GetActorLocation(), FRotator::ZeroRotator, FVector(10.0f));
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		// Volvemos a la normalidad tras 0.5s de tiempo real (usamos temporizador no afectado por la dilatación)
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("FinalizarMuerteCinematica"));
		GetWorldTimerManager().SetTimer(TimerMuerteCinematica, TimerDel, 0.15f, false);
	}
}

void ANaveNodriza::RestaurarMaterial()
{
	if (!bMuerto) SetActorScale3D(FVector(5.0f, 5.0f, 5.0f)); // Vuelve a su tamaño intimidante
}

void ANaveNodriza::FinalizarMuerteCinematica()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); // Restaurar tiempo del motor
	Destroy();
}