// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavesArcadePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

ANavesArcadePawn::ANavesArcadePawn()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;


	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	
	RootComponent = PlaneMesh;

	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	
	SpringArm->TargetArmLength = 160.0f; 
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;	
	SpringArm->CameraLagSpeed = 15.f;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	
	Camera->bUsePawnControlRotation = false; 


	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
}

void ANavesArcadePawn::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);


	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	
	AddActorLocalRotation(DeltaRotation);


	Super::Tick(DeltaSeconds);
}

void ANavesArcadePawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void ANavesArcadePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Thrust", this, &ANavesArcadePawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ANavesArcadePawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANavesArcadePawn::MoveRightInput);
}

void ANavesArcadePawn::ThrustInput(float Val)
{
	
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ANavesArcadePawn::MoveUpInput(float Val)
{
	
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ANavesArcadePawn::MoveRightInput(float Val)
{
	
	float TargetYawSpeed = (Val * TurnSpeed);


	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
