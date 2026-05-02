// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NavesArcadePawn.generated.h"

UCLASS(Config=Game)
class ANavesArcadePawn : public APawn
{
	GENERATED_BODY()

	class UStaticMeshComponent* PlaneMesh;

	class USpringArmComponent* SpringArm;

	class UCameraComponent* Camera;
public:
	ANavesArcadePawn();

	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions

	void ThrustInput(float Val);
	
	void MoveUpInput(float Val);

	void MoveRightInput(float Val);

private:

	float Acceleration;

	float TurnSpeed;

	float MaxSpeed;

	float MinSpeed;

	float CurrentForwardSpeed;

	float CurrentYawSpeed;

	float CurrentPitchSpeed;

	float CurrentRollSpeed;

public:
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
