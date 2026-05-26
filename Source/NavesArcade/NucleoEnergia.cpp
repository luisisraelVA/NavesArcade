#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"

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

    // Usar esfera en lugar de toro (el toro no existe en UE4.27)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereMesh.Succeeded())
    {
        MallaNucleo->SetStaticMesh(SphereMesh.Object);
        MallaNucleo->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f)); // Tamaño visible
    }
    else
    {
        // Fallback extremo (nunca debería ocurrir)
        UE_LOG(LogTemp, Error, TEXT("No se pudo cargar la esfera para el núcleo"));
    }
}

void ANucleoEnergia::BeginPlay()
{
    Super::BeginPlay();
    EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &ANucleoEnergia::AlSuperponerse);
}

void ANucleoEnergia::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ANucleoEnergia::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
    if (Jugador)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("¡NÚCLEO RECOGIDO!"));
        Jugador->RecolectarEnergia(1.0f);
        Destroy();
    }
}