#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NaveAcechadora.h"
#include "DronCentinela.h"
#include "NaveNodriza.h"
#include "NaveJugador.h"

ANucleoEnergia::ANucleoEnergia()
{
	// CORRECCIÓN 1: Dejar el núcleo completamente estático y optimizado sin Tick
	PrimaryActorTick.bCanEverTick = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;

	// Aumentamos el radio de colisión a 500 para que se adapte al nuevo tamaño gigante del cristal
	EsferaColision->InitSphereRadius(500.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true);

	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);
	MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/MisNaves/rock_crystals.rock_crystals'"));
	if (MeshAsset.Succeeded())
	{
		MallaNucleo->SetStaticMesh(MeshAsset.Object);
		// Forzamos el tamaño masivo para compensar el tamaño microscópico del asset
		MallaNucleo->SetRelativeScale3D(FVector(50.0f, 50.0f, 50.0f));
	}
}

void ANucleoEnergia::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ANucleoEnergia::AlSuperponerse);

	// CORRECCIÓN 2: ALGORITMO DE RECENTRADO AUTOMÁTICO PARA UE 4.27
	// Forzamos a la malla visual a alinearse perfectamente con la posición real del Actor
	if (MallaNucleo)
	{
		// 1. Obtenemos el centro real de la geometría en el espacio del mundo
		FVector CentroMundo = MallaNucleo->Bounds.Origin;
		FVector UbicacionActor = GetActorLocation();

		// 2. Calculamos el vector de desfase causado por el pivote roto
		FVector DesfaseMundo = CentroMundo - UbicacionActor;

		// 3. Transformamos el desfase a espacio local y lo restamos para centrar el modelo visual
		FVector DesfaseLocal = GetActorTransform().InverseTransformVector(DesfaseMundo);
		MallaNucleo->SetRelativeLocation(-DesfaseLocal);
	}
}

void ANucleoEnergia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANucleoEnergia::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
	if (Jugador)
	{
		// 1. Escaneamos si quedan naves enemigas en todo el nivel
		TArray<AActor*> Acechadoras;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANaveAcechadora::StaticClass(), Acechadoras);

		TArray<AActor*> Drones;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADronCentinela::StaticClass(), Drones);

		TArray<AActor*> Jefes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANaveNodriza::StaticClass(), Jefes);

		int32 EnemigosVivos = Acechadoras.Num() + Drones.Num() + Jefes.Num();

		// 2. Lógica de Bloqueo / Recolección
		if (EnemigosVivos > 0)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("ACCESO DENEGADO: ELIMINA A LOS GUARDIANES PRIMERO"));
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, TEXT("NÚCLEO ASEGURADO"));
			Jugador->RecolectarEnergia(1.0f);
			Destroy();
		}
	}
}