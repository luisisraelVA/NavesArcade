#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelBuilder.h"
#include "GameAssets.h"
#include "NaveJugador.h"
#include "AudioManager.h"

ANucleoEnergia::ANucleoEnergia()
{
	PrimaryActorTick.bCanEverTick = false; 

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
		// SOLUCIÓN: Buscamos el audio de la NAVE y hacemos que la nave haga el sonido
		UAudioManager* AudioJugador = Cast<UAudioManager>(Jugador->GetComponentByClass(UAudioManager::StaticClass()));
		if (AudioJugador)
		{
			AudioJugador->PlaySoundRecogerNucleo();
		}

		Jugador->RecolectarEnergia(1.0f);

		ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
		if (Builder) Builder->SetNucleoPendiente(false);

		Destroy(); // Ahora sí podemos destruir el núcleo tranquilos
	}
}