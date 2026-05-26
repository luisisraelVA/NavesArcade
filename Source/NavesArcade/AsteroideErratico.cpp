#include "AsteroideErratico.h"

AAsteroideErratico::AAsteroideErratico()
{
    PrimaryActorTick.bCanEverTick = true;
    FrecuenciaOndulacion = 5.0f;
    AmplitudOndulacion = 300.0f;
    TiempoAcumulado = 0.0f;
}

void AAsteroideErratico::BeginPlay()
{
    Super::BeginPlay();
}

void AAsteroideErratico::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TiempoAcumulado += DeltaTime;
    FVector Desviacion = FVector(0.0f, FMath::Sin(TiempoAcumulado * FrecuenciaOndulacion) * AmplitudOndulacion * DeltaTime, 0.0f);
    AddActorWorldOffset(Desviacion, true);
}