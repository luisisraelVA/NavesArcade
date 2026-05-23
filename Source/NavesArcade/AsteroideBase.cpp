#include "AsteroideBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"

AAsteroideBase::AAsteroideBase()
{
	PrimaryActorTick.bCanEverTick = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(120.0f); // Un poco más grande para la roca

	MallaAsteroide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAsteroide"));
	MallaAsteroide->SetupAttachment(RootComponent);

	// C++ PURO: Cargamos la malla de una roca real del StarterContent del motor
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaRoca(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Rock.SM_Rock'"));
	if (MallaRoca.Succeeded())
	{
		MallaAsteroide->SetStaticMesh(MallaRoca.Object);
		MallaAsteroide->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); // Escala para que intimide
	}

	// CORRECCIÓN CRÍTICA: Bloqueo físico real para que la nave NO atraviese el asteroide como fantasma
	EsferaColision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true); // Permite que se siga llamando la función de daño

	DanoPorChoque = 25.0f;
}

void AAsteroideBase::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AAsteroideBase::AlSuperponerse);
}

void AAsteroideBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAsteroideBase::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ANaveJugador* NaveChocada = Cast<ANaveJugador>(OtherActor);
		if (NaveChocada)
		{
			NaveChocada->RecibirDano(DanoPorChoque);
			Destroy();
		}
	}
}
