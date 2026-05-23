#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "AsteroideBase.h"
#include "SpawnVolume.h"

AProyectil::AProyectil()
{
	PrimaryActorTick.bCanEverTick = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	EsferaColision->InitSphereRadius(15.0f);
	EsferaColision->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = EsferaColision;

	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	MallaProyectil->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
		MallaProyectil->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	}

	MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
	MovimientoProyectil->UpdatedComponent = EsferaColision;
	MovimientoProyectil->InitialSpeed = 3000.0f;
	MovimientoProyectil->MaxSpeed = 3000.0f;
	MovimientoProyectil->bRotationFollowsVelocity = true;
	MovimientoProyectil->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 3.0f;
}

void AProyectil::BeginPlay()
{
	Super::BeginPlay();
	if (EsferaColision)
	{
		EsferaColision->OnComponentHit.AddDynamic(this, &AProyectil::AlImpactar);
	}
}

void AProyectil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProyectil::AlImpactar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		// Si choca contra las cajas invisibles de spawn, las ignoramos para no destruir el disparo en la nada
		if (OtherActor->IsA(ASpawnVolume::StaticClass()))
		{
			return;
		}

		// Si impacta contra cualquier tipo de asteroide (Base, Dinámico, Errático o Explosivo) lo destruimos
		if (OtherActor->IsA(AAsteroideBase::StaticClass()))
		{
			OtherActor->Destroy();
		}

		Destroy();
	}
}
