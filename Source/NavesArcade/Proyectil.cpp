#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

AProyectil::AProyectil()
{
	PrimaryActorTick.bCanEverTick = false;

	// Inicialización de Colisión
	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	EsferaColision->InitSphereRadius(15.0f);
	EsferaColision->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = EsferaColision;

	// Inicialización de Malla
	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	MallaProyectil->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
		MallaProyectil->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	}

	// Inicialización de Movimiento
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
		Destroy();
	}
}
