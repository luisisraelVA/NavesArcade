#include "AsteroideBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"

AAsteroideBase::AAsteroideBase()
{
    PrimaryActorTick.bCanEverTick = true;

    EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
    RootComponent = EsferaColision;
    EsferaColision->InitSphereRadius(100.0f);
    EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    EsferaColision->SetGenerateOverlapEvents(true);

    MallaAsteroide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAsteroide"));
    MallaAsteroide->SetupAttachment(RootComponent);
    MallaAsteroide->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> EsferaMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (EsferaMesh.Succeeded())
    {
        MallaAsteroide->SetStaticMesh(EsferaMesh.Object);
        MallaAsteroide->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
    }

    DanoPorChoque = 25.0f;
}

void AAsteroideBase::BeginPlay()
{
    Super::BeginPlay();
    EsferaColision->OnComponentBeginOverlap.AddDynamic(this, &AAsteroideBase::AlSuperponerse);

    EscalaOriginal = GetActorScale3D();
    SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));
    TiempoVisual = 0.0f;
}

void AAsteroideBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TiempoVisual += DeltaTime;

    if (TiempoVisual <= 1.5f)
    {
        FVector EscalaSuave = FMath::VInterpTo(GetActorScale3D(), EscalaOriginal, DeltaTime, 4.0f);
        SetActorScale3D(EscalaSuave);
    }
    else if (TiempoVisual >= 13.5f)
    {
        FVector EscalaSuave = FMath::VInterpTo(GetActorScale3D(), FVector(0.01f, 0.01f, 0.01f), DeltaTime, 5.0f);
        SetActorScale3D(EscalaSuave);
    }
}

void AAsteroideBase::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        ANaveJugador* Jugador = Cast<ANaveJugador>(OtherActor);
        if (Jugador)
        {
            Jugador->RecibirDano(DanoPorChoque);
            Destroy();
        }
    }
}