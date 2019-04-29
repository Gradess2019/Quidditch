// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "AttachableObject.h"
#include "Components/PrimitiveComponent.h"
#include "Goal.h"
#include "Quaffle.generated.h"

/**
 * 
 */
UCLASS()
class QUIDDITCH_API AQuaffle : public ABall, public IAttachableObject, public IGoal
{
	GENERATED_BODY()

public:

	void Attach(USceneComponent* parentComponent) override;
	void Detach() override;

	int GetPoints() override;
	bool ShouldFinishGame() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Goal interface settings")
	int points;

	UPROPERTY(EditDefaultsOnly, Category = "Goal interface settings")
	bool finishGame;

	FVector lastDeltaLocation;

	FTimerHandle continueMovementTimer;

	void Move() override;
	
	FVector GetNewVelocity() const;
	void NegateAxisValue(float& axisValue) const;
	bool IsMovingAway(const float EXTENT_VALUE, const float CURRENT_DELTA_VALUE, const float LAST_DELTA_VALUE) const;

	bool CorrectDirection(FVector& velocity);
	void GetFlightZoneOriginAndExtent(FVector& flightZoneOrigin, FVector& flightZoneExtent) const;

	void SetVelocity(const FVector& velocity) const;

private:

	void ContinueMovement() const;
};
