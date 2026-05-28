#include "NucleoEnergia.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "LevelBuilder.h"
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

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereMesh.Succeeded())
    {
        MallaNucleo->SetStaticMesh(SphereMesh.Object);
        MallaNucleo->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
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
        Jugador->RecolectarEnergia(1.0f);

        ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
        if (Builder)
        {
            Builder->bNucleoPendiente = false;
            // No llamamos a GenerarFaseObjetivo aquí; RecolectarEnergia ya lo hace si es necesario
        }

        Destroy();
    }
}