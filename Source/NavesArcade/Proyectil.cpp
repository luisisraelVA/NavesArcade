#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystem.h" 
#include "AsteroideBase.h"
#include "DronCentinela.h"
#include "NaveAcechadora.h"
#include "NaveNodriza.h" // NUEVO: Incluimos al jefe final
#include "NaveJugador.h"

AProyectil::AProyectil()
{
	PrimaryActorTick.bCanEverTick = false;

	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	EsferaColision->InitSphereRadius(5.0f);
	EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EsferaColision->SetGenerateOverlapEvents(true);
	RootComponent = EsferaColision;

	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	MallaProyectil->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/MisNaves/pipe_straight.pipe_straight'"));
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
		MallaProyectil->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticulaAsset.Succeeded()) EfectoExplosion = ParticulaAsset.Object;

	MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
	MovimientoProyectil->UpdatedComponent = EsferaColision;
	MovimientoProyectil->InitialSpeed = 4500.0f;
	MovimientoProyectil->MaxSpeed = 4500.0f;
	MovimientoProyectil->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 4.0f;
}

void AProyectil::BeginPlay()
{
	Super::BeginPlay();
	if (EsferaColision) EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectil::AlSuperponerse);
}

void AProyectil::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AProyectil::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;
	if (!OtherComp || !OtherComp->IsA(UStaticMeshComponent::StaticClass())) return;

	bool bDisparadoPorJugador = (GetOwner() && GetOwner()->IsA(ANaveJugador::StaticClass()));

	if (bDisparadoPorJugador)
	{
		// MEJORA: Lógica diferenciada para el Jefe Final
		if (OtherActor->IsA(ANaveNodriza::StaticClass()))
		{
			ANaveNodriza* Boss = Cast<ANaveNodriza>(OtherActor);
			if (Boss) Boss->RecibirDano(25.0f); // Le quitamos vida en lugar de destruirlo
			Destroy(); // El láser sí se destruye
			return;
		}

		if (OtherActor->IsA(AAsteroideBase::StaticClass()) || OtherActor->IsA(ADronCentinela::StaticClass()) || OtherActor->IsA(ANaveAcechadora::StaticClass()))
		{
			if (EfectoExplosion) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoExplosion, OtherActor->GetActorLocation());
			ANaveJugador* Jugador = Cast<ANaveJugador>(GetOwner());
			if (Jugador) Jugador->SumarPuntos(100);

			OtherActor->Destroy();
			Destroy();
		}
	}
	else
	{
		ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
		if (Jugador)
		{
			Jugador->RecibirDano(15.0f);
			Destroy();
		}
		else if (OtherActor->IsA(AAsteroideBase::StaticClass())) Destroy();
	}
}