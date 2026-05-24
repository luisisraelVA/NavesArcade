#include "LevelBuilder.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "NucleoEnergia.h"
#include "EnemyFactory.h"
#include "NaveNodriza.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ALevelBuilder::ALevelBuilder()
{
	PrimaryActorTick.bCanEverTick = false;
	ClaseAsteroide = AAsteroideDinamico::StaticClass();
	ClaseEnergia = ANucleoEnergia::StaticClass();
	DensidadAsteroides = 4;
	VelocidadGlobal = 500.0f;
	FaseActualMision = 0; // Empezamos en la fase 0
}

void ALevelBuilder::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelBuilder::SetFabrica(TScriptInterface<ILevelFab> NuevaFabrica) { FabricaDeFase = NuevaFabrica; }
void ALevelBuilder::SetClaseAsteroide(TSubclassOf<AAsteroideBase> NuevaClase) { ClaseAsteroide = NuevaClase; }

void ALevelBuilder::SpawnAsteroides(int32 Cantidad, float VelocidadAsteroide)
{
	DensidadAsteroides = FMath::Clamp(Cantidad / 25, 2, 5);
	VelocidadGlobal = VelocidadAsteroide;

	// Inicia el mundo de asteroides continuos
	GetWorld()->GetTimerManager().SetTimer(TimerMundoAbierto, this, &ALevelBuilder::GeneracionContinua, 0.6f, true);

	// Iniciamos inmediatamente la primera fase de combate
	GenerarFaseObjetivo();
}

void ALevelBuilder::GenerarFaseObjetivo()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn || !ClaseEnergia || !FabricaDeFase.GetObject()) return;

	ILevelFab* InterfazNativa = Cast<ILevelFab>(FabricaDeFase.GetObject());

	// Calculamos una ubicación a 15,000 unidades en frente de donde esté el jugador AHORA
	FVector UbicacionBase = PlayerPawn->GetActorLocation() + (PlayerPawn->GetActorForwardVector() * 15000.0f);

	// Desviamos un poco para que no esté perfectamente al centro siempre
	UbicacionBase += FVector(0.0f, FMath::RandRange(-3000.0f, 3000.0f), FMath::RandRange(-2000.0f, 2000.0f));

	// Spawneamos el Núcleo Protegido
	GetWorld()->SpawnActor<ANucleoEnergia>(ClaseEnergia, UbicacionBase, FRotator::ZeroRotator);

	// Spawneamos Defensores según la fase
	if (FaseActualMision == 0)
	{
		// Primera base: Fácil (3 enemigos)
		for (int32 i = 0; i < 3; i++)
		{
			FVector PosEnemigo = UbicacionBase + FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-500.f, 500.f));
			InterfazNativa->CrearEnemigo(GetWorld(), PosEnemigo);
		}
	}
	else if (FaseActualMision == 1)
	{
		// Segunda base: Media (5 enemigos)
		for (int32 i = 0; i < 5; i++)
		{
			FVector PosEnemigo = UbicacionBase + FVector(FMath::RandRange(-1500.f, 1500.f), FMath::RandRange(-1500.f, 1500.f), FMath::RandRange(-800.f, 800.f));
			InterfazNativa->CrearEnemigo(GetWorld(), PosEnemigo);
		}
	}
	else if (FaseActualMision == 2)
	{
		// Tercera base: ¡El Jefe Final protege el último núcleo!
		UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Boss, UbicacionBase + FVector(2000.0f, 0.0f, 0.0f));

		// Y 2 súbditos de apoyo
		for (int32 i = 0; i < 2; i++)
		{
			FVector PosEnemigo = UbicacionBase + FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.0f);
			InterfazNativa->CrearEnemigo(GetWorld(), PosEnemigo);
		}
	}

	FaseActualMision++;
}

void ALevelBuilder::GeneracionContinua()
{
	// ESTO SOLO GENERA ASTEROIDES (Cero enemigos aleatorios)
	if (!ClaseAsteroide || !GetWorld()) return;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector PosicionJugador = PlayerPawn->GetActorLocation();
	FVector Frente = PlayerPawn->GetActorForwardVector();
	FVector Derecha = PlayerPawn->GetActorRightVector();
	FVector Arriba = PlayerPawn->GetActorUpVector();

	for (int32 i = 0; i < DensidadAsteroides; i++)
	{
		FVector Loc = PosicionJugador + (Frente * FMath::RandRange(7000.0f, 9000.0f)) + (Derecha * FMath::RandRange(-4500.0f, 4500.0f)) + (Arriba * FMath::RandRange(-3000.0f, 3000.0f));
		FRotator Rot = FRotator(FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f));
		AActor* NuevoRoca = GetWorld()->SpawnActor<AActor>(ClaseAsteroide, Loc, Rot);

		if (NuevoRoca)
		{
			NuevoRoca->SetLifeSpan(12.0f);
			AAsteroideDinamico* AstDinamico = Cast<AAsteroideDinamico>(NuevoRoca);
			if (AstDinamico) AstDinamico->ConfigurarMovimiento(VelocidadGlobal, Frente * -1.0f);
		}
	}
}