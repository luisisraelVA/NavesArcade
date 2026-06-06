#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystem.h" 
#include "NaveJugador.h"
#include "NaveNodriza.h" 
#include "EnemigoBase.h" 
#include "AsteroideBase.h"
#include "GameAssets.h" 

AProyectil::AProyectil()
{
	PrimaryActorTick.bCanEverTick = false; // Apagamos el motor continuo

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	EsferaColision->InitSphereRadius(40.0f);
	EsferaColision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EsferaColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	EsferaColision->SetGenerateOverlapEvents(true);
	EsferaColision->BodyInstance.bUseCCD = true;
	RootComponent = EsferaColision;

	// Vinculación en el constructor para evitar bugs de cambio de mapa
	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectil::AlSuperponerse);

	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	MallaProyectil->SetupAttachment(RootComponent);
	MallaProyectil->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(GameAssets::MallaProyectil);
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
		MallaProyectil->SetRelativeScale3D(FVector(0.4f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(GameAssets::EfectoExplosion);
	if (ParticulaAsset.Succeeded()) EfectoExplosion = ParticulaAsset.Object;

	MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
	MovimientoProyectil->UpdatedComponent = EsferaColision;
	MovimientoProyectil->InitialSpeed = 4500.0f;
	MovimientoProyectil->MaxSpeed = 4500.0f;
	MovimientoProyectil->ProjectileGravityScale = 0.0f;
	MovimientoProyectil->bSweepCollision = true;

	InitialLifeSpan = 5.0f;
}

void AProyectil::BeginPlay()
{
	Super::BeginPlay();
}

void AProyectil::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

	bool bDisparadoPorJugador = (GetOwner() && GetOwner()->IsA(ANaveJugador::StaticClass()));

	if (bDisparadoPorJugador)
	{
		if (AEnemigoBase* EnemigoGolpeado = Cast<AEnemigoBase>(OtherActor))
		{
			if (EfectoExplosion) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoExplosion, OtherActor->GetActorLocation());
			EnemigoGolpeado->RecibirDano(10.0f);
			ANaveJugador* Jugador = Cast<ANaveJugador>(GetOwner());
			if (Jugador) Jugador->SumarPuntos(100);
			Destroy();
		}
		else if (OtherActor->IsA(AAsteroideBase::StaticClass()))
		{
			if (EfectoExplosion) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoExplosion, OtherActor->GetActorLocation());
			OtherActor->Destroy();
			Destroy();
		}
	}
	else
	{
		if (ANaveJugador* JugadorGolpeado = Cast<ANaveJugador>(OtherActor))
		{
			JugadorGolpeado->RecibirDano(15.0f);
			Destroy();
		}
		else if (OtherActor->IsA(AAsteroideBase::StaticClass())) Destroy();
	}
}