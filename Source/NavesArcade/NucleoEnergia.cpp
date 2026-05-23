
#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"
#include "NaveFacade.h"

ANucleoEnergia::ANucleoEnergia()
{
	PrimaryActorTick.bCanEverTick = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(80.0f);

	// CORRECCIÓN FÍSICA: Aseguramos que responda tanto a bloqueos como a superposiciones
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true); // OBLIGATORIO: Fuerza a Unreal a escuchar el choque

	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);

	// CORRECCIÓN DE MALLA: Nos aseguramos de que la estética del cristal no bloquee las físicas de la esfera
	MallaNucleo->SetCollisionProfileName(TEXT("NoCollision"));
	MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// C++ PURO: Cargamos una forma de pilar/cristal del motor para que parezca energía pura
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCristal(TEXT("StaticMesh'/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500'"));
	if (MallaCristal.Succeeded())
	{
		MallaNucleo->SetStaticMesh(MallaCristal.Object);
		MallaNucleo->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f)); // Lo encogemos para que sea un ítem flotante
	}

	ValorEnergia = 20.0f;
}

void ANucleoEnergia::BeginPlay()
{
	Super::BeginPlay();

	// Aseguramos el enlace dinámico nativo de manera estricta
	if (EsferaColision)
	{
		EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ANucleoEnergia::AlSuperponerse);
	}
}

void ANucleoEnergia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANucleoEnergia::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ANaveJugador* Nave = Cast<ANaveJugador>(OtherActor);
		if (Nave)
		{
			UNaveFacade* Facade = Nave->FindComponentByClass<UNaveFacade>();
			if (Facade)
			{
				Facade->ProcesarRecoleccionEnergia(ValorEnergia);
				Destroy();
			}
		}
	}
}