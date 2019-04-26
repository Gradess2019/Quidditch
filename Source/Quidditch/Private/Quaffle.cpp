// Fill out your copyright notice in the Description page of Project Settings.

#include "Quaffle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "QuidditchHelper.h"
#include "TimerManager.h"

FVector GetFlightZoneOrigin(AActor* flightZone);
FVector GetFlightZoneExtent(AActor* flightZone);

void AQuaffle::Move()
{
	FVector newVelocity = GetNewVelocity();
	CorrectDirection(newVelocity);
	SetVelocity(newVelocity);
}

FVector AQuaffle::GetNewVelocity()
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
	const float PADDING = 100.f;

	bool wasCorrected = false;

	const FVector currentLocation = GetActorLocation();
	const FVector flightZoneOrigin = GetFlightZoneOrigin(flightZone);
	const FVector extent = GetFlightZoneExtent(flightZone);
	const FVector deltaLocation = (flightZoneOrigin - currentLocation).GetAbs();


	if (!lastDeltaLocation.IsZero())
	{
		if (extent.X < deltaLocation.X && deltaLocation.X > lastDeltaLocation.X)
		{
			velocity.X *= -1;
			wasCorrected = true;
		}

		if (extent.Y < deltaLocation.Y && deltaLocation.Y > lastDeltaLocation.Y)
		{
			velocity.Y *= -1;
			wasCorrected = true;
		}

		if (extent.Z < deltaLocation.Z && deltaLocation.Z > lastDeltaLocation.Z)
		{
			velocity.Z *= -1;
			wasCorrected = true;
		}
	}

	lastDeltaLocation = deltaLocation;

	return wasCorrected;
}

FVector GetFlightZoneOrigin(AActor* flightZone)
{
	FVector flightZoneOrigin;
	FVector zero = FVector::ZeroVector;
	flightZone->GetActorBounds(false, flightZoneOrigin, zero);
	return flightZoneOrigin;
}

FVector GetFlightZoneExtent(AActor* flightZone)
{
	FVector flightZoneExtent;
	FVector zero = FVector::ZeroVector;
	flightZone->GetActorBounds(false, zero, flightZoneExtent);
	return flightZoneExtent;
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
	GetWorld()->GetTimerManager().SetTimer(continueMovementTimer, this, &AQuaffle::ContinueMovement, 5.f, false);
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