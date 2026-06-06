#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelBuilder.h"
#include "GameAssets.h"
#include "NaveJugador.h"

ANucleoEnergia::ANucleoEnergia()
{
	PrimaryActorTick.bCanEverTick = false; // Apagamos el cerebro del actor

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(300.0f);
	EsferaColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	EsferaColision->SetGenerateOverlapEvents(true);

	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);
	MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(GameAssets::MallaNucleoEnergia);
	if (MeshAsset.Succeeded())
	{
		MallaNucleo->SetStaticMesh(MeshAsset.Object);
		MallaNucleo->SetRelativeScale3D(FVector(0.5f));
	}
}

void ANucleoEnergia::BeginPlay()
{
	Super::BeginPlay();
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ANucleoEnergia::AlSuperponerse);
}

void ANucleoEnergia::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
	if (Jugador)
	{
		Jugador->RecolectarEnergia(1.0f);

		ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
		if (Builder) Builder->bNucleoPendiente = false;

		Destroy();
	}
}