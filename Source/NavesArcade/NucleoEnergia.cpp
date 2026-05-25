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
	PrimaryActorTick.bCanEverTick = true;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(150.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true);

	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);
	MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/MisNaves/rock_crystals.rock_crystals'"));
	if (MeshAsset.Succeeded()) MallaNucleo->SetStaticMesh(MeshAsset.Object);

	MallaNucleo->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void ANucleoEnergia::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ANucleoEnergia::AlSuperponerse);
}

void ANucleoEnergia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Rotación vistosa
	AddActorLocalRotation(FRotator(50.f * DeltaTime, 100.f * DeltaTime, 0.f));
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
			// Si hay enemigos, el escudo está activo. Le avisamos al jugador en pantalla roja.
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("ACCESO DENEGADO: ELIMINA A LOS GUARDIANES PRIMERO"));
		}
		else
		{
			// Si limpió la zona, recolectamos
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, TEXT("NÚCLEO ASEGURADO"));
			Jugador->RecolectarEnergia(1.0f);
			Destroy();
		}
	}
}