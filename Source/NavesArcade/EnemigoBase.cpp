#include "EnemigoBase.h"
#include "Components/SphereComponent.h"
#include "LevelBuilder.h"
#include "NavesArcadeGameMode.h"
#include "Kismet/GameplayStatics.h"

AEnemigoBase::AEnemigoBase()
{
    PrimaryActorTick.bCanEverTick = true;
    EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
    RootComponent = EsferaColision;
    EsferaColision->InitSphereRadius(80.0f);                     
    EsferaColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    EsferaColision->SetGenerateOverlapEvents(true);

    Escudo = 0.0f;
    Salud = 20.0f;
    Tags.Add(FName("Enemy"));
}

void AEnemigoBase::BeginPlay()
{
    Super::BeginPlay();
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->RegistrarEnemigo(this);
}

void AEnemigoBase::RecibirDano(float Cantidad)
{
    if (Escudo > 0)
    {
        Escudo -= Cantidad;
        if (Escudo < 0)
        {
            Salud -= FMath::Abs(Escudo);
            Escudo = 0;
        }
    }
    else
    {
        Salud -= Cantidad;
    }

    if (Salud <= 0) Destroy();
}

void AEnemigoBase::Destroyed()
{
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->DesregistrarEnemigo(this);

    ALevelBuilder* Builder = Cast<ALevelBuilder>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelBuilder::StaticClass()));
    if (Builder) Builder->NotificarMuerteEnemigo();

    Super::Destroyed();
}