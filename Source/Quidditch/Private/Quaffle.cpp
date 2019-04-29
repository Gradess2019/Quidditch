// Fill out your copyright notice in the Description page of Project Settings.

#include "Quaffle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "QuidditchHelper.h"
#include "TimerManager.h"

void AQuaffle::Move()
{
	FVector newVelocity = GetNewVelocity();
	CorrectDirection(newVelocity);
	SetVelocity(newVelocity);
}

FVector AQuaffle::GetNewVelocity() const
{
	const FVector currentVelocity = this->GetVelocity();

	if (currentVelocity.IsZero())
	{
		return UQuidditchHelper::GetRandomVelocity();
	}
	else
	{
		const float INTERP_SPEED = 300.f;
		return UQuidditchHelper::InterpolateToTargetVelocity(this, maxSpeed, INTERP_SPEED);
	}
}

bool AQuaffle::CorrectDirection(FVector& velocity)
{
	bool wasCorrected = false;

	const FVector CURRENT_LOCATION = GetActorLocation();

	FVector flightZoneOrigin = FVector::ZeroVector;
	FVector flightZoneExtent = FVector::ZeroVector;

	GetFlightZoneOriginAndExtent(flightZoneOrigin, flightZoneExtent);

	const FVector CURRENT_DELTA_LOCATION = (flightZoneOrigin - CURRENT_LOCATION).GetAbs();


	if (!lastDeltaLocation.IsZero())
	{
		if (IsMovingAway(flightZoneExtent.X, CURRENT_DELTA_LOCATION.X, lastDeltaLocation.X))
		{
			NegateAxisValue(velocity.X);
			wasCorrected = true;
		}

		if (IsMovingAway(flightZoneExtent.Y, CURRENT_DELTA_LOCATION.Y, lastDeltaLocation.Y))
		{
			NegateAxisValue(velocity.Y);
			wasCorrected = true;
		}

		if (IsMovingAway(flightZoneExtent.Z, CURRENT_DELTA_LOCATION.Z, lastDeltaLocation.Z))
		{
			NegateAxisValue(velocity.Z);
			wasCorrected = true;
		}
	}

	lastDeltaLocation = CURRENT_DELTA_LOCATION;

	return wasCorrected;
}


void AQuaffle::GetFlightZoneOriginAndExtent(FVector& flightZoneOrigin, FVector& flightZoneExtent) const
{
	flightZone->GetActorBounds(false, flightZoneOrigin, flightZoneExtent);
}

bool AQuaffle::IsMovingAway(const float EXTENT_VALUE, const float CURRENT_DELTA_VALUE, const float LAST_DELTA_VALUE) const
{
	return EXTENT_VALUE < CURRENT_DELTA_VALUE && CURRENT_DELTA_VALUE > LAST_DELTA_VALUE;
}

void AQuaffle::NegateAxisValue(float& axisValue) const
{
	axisValue *= -1;
}

void AQuaffle::SetVelocity(const FVector& velocity) const
{
	physicalComponent->SetPhysicsLinearVelocity(velocity);
}

void AQuaffle::Attach(USceneComponent* parentComponent)
{
	GetWorld()->GetTimerManager().PauseTimer(movementUpdateTimer);
	physicalComponent->SetSimulatePhysics(false);
	AttachToComponent(parentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AQuaffle::Detach()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	physicalComponent->SetSimulatePhysics(true);

	const float DELAY = 5.f;
	const bool IS_LOOP = false;
	GetWorld()->GetTimerManager().SetTimer(continueMovementTimer, this, &AQuaffle::ContinueMovement, DELAY, IS_LOOP);
}

void AQuaffle::ContinueMovement() const
{
	GetWorld()->GetTimerManager().UnPauseTimer(movementUpdateTimer);
}

int AQuaffle::GetPoints()
{
	return points;
}

bool AQuaffle::ShouldFinishGame()
{
	return finishGame;
}