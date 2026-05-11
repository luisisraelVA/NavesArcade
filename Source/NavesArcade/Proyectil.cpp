#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProyectil::AProyectil()
{
	PrimaryActorTick.bCanEverTick = true;

	Colision = CreateDefaultSubobject<USphereComponent>(TEXT("Colision"));
	RootComponent = Colision;
	Colision->InitSphereRadius(15.0f);

	Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
	Malla->SetupAttachment(RootComponent);

	Movimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movimiento"));
	Movimiento->UpdatedComponent = Colision;
	Movimiento->InitialSpeed = 3000.f;
	Movimiento->MaxSpeed = 3000.f;
	Movimiento->bRotationFollowsVelocity = true;
	Movimiento->bShouldBounce = false;
}

void AProyectil::BeginPlay() { Super::BeginPlay(); }
void AProyectil::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
