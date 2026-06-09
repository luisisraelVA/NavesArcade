#include "PortalSalto.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NaveJugador.h"
#include "GameAssets.h"
#include "NavesArcadeGameMode.h"
#include "Kismet/GameplayStatics.h"

APortalSalto::APortalSalto()
{
	PrimaryActorTick.bCanEverTick = false; 
	bViajeIniciado = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision;
	EsferaColision->InitSphereRadius(1000.0f);
	EsferaColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	EsferaColision->SetGenerateOverlapEvents(true);

	MallaSuperficie = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaSuperficie"));
	MallaSuperficie->SetupAttachment(RootComponent);
	MallaSuperficie->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MallaSuperficieTrasera = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaSuperficieTrasera"));
	MallaSuperficieTrasera->SetupAttachment(RootComponent);
	MallaSuperficieTrasera->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MallaSuperficieTrasera->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSuperficie(GameAssets::MallaPortalSuperficie);
	if (MeshSuperficie.Succeeded())
	{
		MallaSuperficie->SetStaticMesh(MeshSuperficie.Object);
		MallaSuperficie->SetRelativeScale3D(FVector(15.0f));

		MallaSuperficieTrasera->SetStaticMesh(MeshSuperficie.Object);
		MallaSuperficieTrasera->SetRelativeScale3D(FVector(15.0f));
	}
}

void APortalSalto::BeginPlay()
{
	Super::BeginPlay();
}

void APortalSalto::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bViajeIniciado) return;

	ANaveJugador* Nave = Cast<ANaveJugador>(OtherActor);
	if (!Nave) return;

	ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
	int32 Requeridos = GM ? GM->GetNucleosRequeridos() : 1;
	int32 Nucleos = Nave->GetNucleosRecolectados();

	if (Nucleos >= Requeridos)
	{
		bViajeIniciado = true;
		SetActorEnableCollision(false);

		if (GM) GM->AvanzarSiguienteNivel();
		else UGameplayStatics::OpenLevel(GetWorld(), FName("Nivel-01"));
	}
}