#include "DronSuicida.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"
#include "GameAssets.h"
#include "Sound/SoundBase.h"

ADronSuicida::ADronSuicida()
{
    EsferaColision->SetSphereRadius(50.0f);
    EsferaColision->SetGenerateOverlapEvents(true);

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    Malla->SetupAttachment(RootComponent);
    Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(GameAssets::MallaDronSuicida);
    if (SphereMesh.Succeeded())
    {
      
            Malla->SetStaticMesh(SphereMesh.Object);
            // Ajusta la escala si la nave no se ve bien
            Malla->SetRelativeScale3D(FVector(0.08f));
            // Ajusta la rotación si la nave no mira hacia adelante
            Malla->SetRelativeRotation(FRotator(0, -90, 0));
       
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> AudioImpactoObj(TEXT("SoundWave'/Game/Sonidos/impacto.impacto'"));
    if (AudioImpactoObj.Succeeded())
    {
        SonidoImpactoDron = AudioImpactoObj.Object;
    }

    VelocidadCarga = 700.0f;
    DistanciaDeteccion = 2000.0f;
    DanoExplosion = 50.0f;
    bActivado = false;
}

void ADronSuicida::BeginPlay()
{
    Super::BeginPlay();
    Objetivo = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ADronSuicida::AlImpactar);
}

void ADronSuicida::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Objetivo) return;

    float Dist = FVector::Dist(GetActorLocation(), Objetivo->GetActorLocation());
    if (Dist < DistanciaDeteccion || bActivado)
    {
        bActivado = true;
        FVector Dir = (Objetivo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorRotation(Dir.Rotation());
        AddActorWorldOffset(Dir * VelocidadCarga * DeltaTime);
    }
}

void ADronSuicida::AlImpactar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
    if (Jugador)
    {
        // 1. Reproducimos el sonido en la ubicación exacta del choque
        if (SonidoImpactoDron)
        {
            UGameplayStatics::PlaySoundAtLocation(this, SonidoImpactoDron, GetActorLocation());
        }

        // 2. Aplicamos el daño al jugador
        Jugador->RecibirDano(DanoExplosion);

        // 3. Nos destruimos
        Destroy();
    }
}