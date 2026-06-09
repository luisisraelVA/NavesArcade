#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h" // AÑADIDO DE VUELTA
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystem.h" 
#include "NaveJugador.h"
#include "NaveNodriza.h" 
#include "EnemigoBase.h" 
#include "AsteroideBase.h"
#include "GameAssets.h" 
#include "AudioManager.h" 

// Puntero estatico para asegurar que el sonido este cargado globalmente (Mantenemos logica anterior)
static USoundBase* SonidoChoqueEnemigoEstatico = nullptr;

AProyectil::AProyectil()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. COLISIÓN (Se mantiene igual)
	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	EsferaColision->InitSphereRadius(40.0f);
	EsferaColision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EsferaColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	EsferaColision->SetGenerateOverlapEvents(true);
	EsferaColision->BodyInstance.bUseCCD = true;
	RootComponent = EsferaColision;

	EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectil::AlSuperponerse);

	// 2. VISUAL: Configuración de la Malla LazerBullet
	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	MallaProyectil->SetupAttachment(RootComponent);
	MallaProyectil->SetCollisionEnabled(ECollisionEnabled::NoCollision); // La malla no colisiona, solo la esfera

	// --- CARGA DE TU MALLA ESPECÍFICA ---
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/MallasFinales/LazerBullet.LazerBullet'"));
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
		// Ajustamos la escala para que no sea gigante comparada con la nave
		MallaProyectil->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

		// Opcional: Si la malla sale rotada por defecto, ajustala aquí:
		// MallaProyectil->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f)); 
	}

	// 3. ASSETS DE SEGURIDAD (Mantenemos lógica anterior de sonidos y partículas de impacto)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(GameAssets::EfectoExplosion);
	if (ParticulaAsset.Succeeded()) EfectoExplosion = ParticulaAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> AudioChoqueEnemigoObj(TEXT("SoundWave'/Game/Sonidos/ChoqueEnemigos.ChoqueEnemigos'"));
	if (AudioChoqueEnemigoObj.Succeeded())
	{
		SonidoChoqueEnemigoEstatico = AudioChoqueEnemigoObj.Object;
	}

	// 4. MOVIMIENTO (Se mantiene igual)
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

// --- MANTENEMOS TODA LA LÓGICA DE COLISIÓN COMPLEJA DESARROLLADA ANTERIORMENTE ---
void AProyectil::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

	bool bDisparadoPorJugador = (GetOwner() && GetOwner()->IsA(ANaveJugador::StaticClass()));

	if (bDisparadoPorJugador)
	{
		// VERIFICACION TRIPLE DE SEGURIDAD PARA ENEMIGOS
		if (OtherActor->ActorHasTag(FName("Enemy")) || OtherActor->IsA(AEnemigoBase::StaticClass()))
		{
			// Reproduccion directa del sonido estatico (Fix para naves con componentes corruptos)
			if (SonidoChoqueEnemigoEstatico)
			{
				UGameplayStatics::PlaySoundAtLocation(this, SonidoChoqueEnemigoEstatico, OtherActor->GetActorLocation());
			}

			if (EfectoExplosion) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoExplosion, OtherActor->GetActorLocation());

			AEnemigoBase* EnemigoGolpeado = Cast<AEnemigoBase>(OtherActor);
			if (EnemigoGolpeado) EnemigoGolpeado->RecibirDano(10.0f);

			ANaveJugador* Jugador = Cast<ANaveJugador>(GetOwner());
			if (Jugador) Jugador->SumarPuntos(100);

			Destroy();
		}
		else if (OtherActor->IsA(AAsteroideBase::StaticClass()))
		{
			if (EfectoExplosion) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfectoExplosion, OtherActor->GetActorLocation());

			// Sonido de impacto genérico desde el manager del asteroide
			UAudioManager* ManagerAudio = Cast<UAudioManager>(OtherActor->GetComponentByClass(UAudioManager::StaticClass()));
			if (ManagerAudio) ManagerAudio->PlaySoundImpacto();

			OtherActor->Destroy();
			Destroy();
		}
	}
	else
	{
		// ENEMIGO LE DA AL JUGADOR
		if (ANaveJugador* JugadorGolpeado = Cast<ANaveJugador>(OtherActor))
		{
			// Sonido de impacto desde el manager del jugador
			UAudioManager* AudioJugador = Cast<UAudioManager>(JugadorGolpeado->GetComponentByClass(UAudioManager::StaticClass()));
			if (AudioJugador) AudioJugador->PlaySoundImpacto();

			JugadorGolpeado->RecibirDano(15.0f);
			Destroy();
		}
		else if (OtherActor->IsA(AAsteroideBase::StaticClass())) Destroy();
	}
}