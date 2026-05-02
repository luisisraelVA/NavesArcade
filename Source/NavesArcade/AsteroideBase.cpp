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

	EsferaColision->InitSphereRadius(100.0f);

	MallaAsteroide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAsteroide"));
	MallaAsteroide->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaEsfera(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MallaEsfera.Succeeded())
	{
		MallaAsteroide->SetStaticMesh(MallaEsfera.Object);
	}

	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

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