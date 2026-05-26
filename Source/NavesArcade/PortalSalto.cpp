#include "PortalSalto.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"
#include "Kismet/GameplayStatics.h"

APortalSalto::APortalSalto()
{
    PrimaryActorTick.bCanEverTick = false;

    EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
    RootComponent = EsferaColision;
    EsferaColision->InitSphereRadius(1000.0f);
    EsferaColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    EsferaColision->SetGenerateOverlapEvents(true);

    MallaPortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPortal"));
    MallaPortal->SetupAttachment(RootComponent);
    MallaPortal->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (CubeMesh.Succeeded())
    {
        MallaPortal->SetStaticMesh(CubeMesh.Object);
        MallaPortal->SetRelativeScale3D(FVector(10.0f, 10.0f, 2.0f));
    }
}

void APortalSalto::BeginPlay()
{
    Super::BeginPlay();
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("PORTAL CREADO"));
}

void APortalSalto::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void APortalSalto::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    ANaveJugador* Nave = Cast<ANaveJugador>(OtherActor);
    if (!Nave) return;
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("¡NAVE TOCÓ PORTAL!"));

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    int32 Requeridos = GM ? GM->GetNucleosRequeridos() : 3;
    int32 Nucleos = Nave->GetNucleosRecolectados();
    if (Nucleos >= Requeridos)
    {
        if (GM) GM->AvanzarSiguienteNivel();
        else UGameplayStatics::OpenLevel(GetWorld(), "Nivel-01");
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Núcleos: %d/%d"), Nucleos, Requeridos));
    }
}