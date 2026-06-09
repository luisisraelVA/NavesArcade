#include "AsteroideBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "GameAssets.h"
#include "AudioManager.h"
#include "NaveJugador.h"

AAsteroideBase::AAsteroideBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(100.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true);

	MallaAsteroide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAsteroideVisual"));
	MallaAsteroide->SetupAttachment(RootComponent);
	MallaAsteroide->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	DanoPorChoque = 25.0f;
}

void AAsteroideBase::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AAsteroideBase::AlSuperponerse);

	EscalaOriginal = GetActorScale3D();
	SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));
	TiempoVisual = 0.0f;

	GetWorldTimerManager().SetTimer(TimerDespertar, this, &AAsteroideBase::ReactivarTick, 10.5f, false);
}

void AAsteroideBase::ReactivarTick()
{
	SetActorTickEnabled(true);
}

void AAsteroideBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TiempoVisual += DeltaTime;

	if (TiempoVisual <= 1.5f)
	{
		FVector EscalaSuave = FMath::VInterpTo(GetActorScale3D(), EscalaOriginal, DeltaTime, 4.0f);
		SetActorScale3D(EscalaSuave);

		// APAGADO INTELIGENTE: Si ya creció, lo dormimos para ahorrar CPU
		if (TiempoVisual >= 1.5f) SetActorTickEnabled(false);
	}
	else if (TiempoVisual >= 10.5f)
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
			// SOLUCIÓN: El impacto suena desde la nave justo antes de recibir el daño
			UAudioManager* AudioJugador = Cast<UAudioManager>(Jugador->GetComponentByClass(UAudioManager::StaticClass()));
			if (AudioJugador)
			{
				AudioJugador->PlaySoundImpacto();
			}

			Jugador->RecibirDano(DanoPorChoque);
			Destroy();
		}
	}
}