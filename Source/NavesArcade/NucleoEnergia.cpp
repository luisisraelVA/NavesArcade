


#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"

ANucleoEnergia::ANucleoEnergia()
{
	PrimaryActorTick.bCanEverTick = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(80.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);
	MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCubo(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MallaCubo.Succeeded())
	{
		MallaNucleo->SetStaticMesh(MallaCubo.Object);
	}

	ValorEnergia = 20.0f; 

}


void ANucleoEnergia::BeginPlay()
{
	Super::BeginPlay();
	
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ANucleoEnergia::AlSuperponerse);
}

void ANucleoEnergia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANucleoEnergia::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		
		ANaveJugador* NaveChocada = Cast<ANaveJugador>(OtherActor);

		if (NaveChocada)
		{
			
			NaveChocada->RecolectarEnergia(ValorEnergia);

			
			Destroy();
		}
	}
}

