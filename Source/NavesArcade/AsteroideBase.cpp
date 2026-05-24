#include "AsteroideBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"

AAsteroideBase::AAsteroideBase()
{
	// ARREGLO MAESTRO: Activamos el Tick para que la roca pueda animarse a sí misma
	PrimaryActorTick.bCanEverTick = true;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(100.0f);

	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true);

	MallaAsteroide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAsteroide"));
	MallaAsteroide->SetupAttachment(RootComponent);
	MallaAsteroide->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaRoca(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Rock.SM_Rock'"));
	if (MallaRoca.Succeeded())
	{
		MallaAsteroide->SetStaticMesh(MallaRoca.Object);
		MallaAsteroide->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	}

	DanoPorChoque = 25.0f;
}

void AAsteroideBase::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AAsteroideBase::AlSuperponerse);

	// PULIDO VISUAL: Guardamos la escala real y hacemos la roca casi invisible
	EscalaOriginal = GetActorScale3D();
	SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));
	TiempoVisual = 0.0f;
}

void AAsteroideBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TiempoVisual += DeltaTime;

	// FASE 1: Apareciendo (Crecen rápidamente durante el primer segundo y medio)
	if (TiempoVisual <= 1.5f)
	{
		FVector EscalaSuave = FMath::VInterpTo(GetActorScale3D(), EscalaOriginal, DeltaTime, 4.0f);
		SetActorScale3D(EscalaSuave);
	}
	// FASE 2: Desapareciendo (Se encogen un segundo antes de que el LifeSpan de 15s los elimine)
	else if (TiempoVisual >= 13.5f)
	{
		FVector EscalaSuave = FMath::VInterpTo(GetActorScale3D(), FVector(0.01f, 0.01f, 0.01f), DeltaTime, 5.0f);
		SetActorScale3D(EscalaSuave);
	}
}

void AAsteroideBase::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
		if (Jugador)
		{
			Jugador->RecibirDano(DanoPorChoque);
			Destroy();
		}
	}
}