// Apache License. Copyright 2019 RTU IT Lab

#include "Snitch.h"
#include "QuidditchHelper.h"
#include "Kismet/KismetMathLibrary.h"

void ASnitch::Move()
{
	FVector newVelocity = GetNewVelocity();

	const bool WAS_CORRECTED = CorrectDirection(newVelocity);

	if (!WAS_CORRECTED)
	{
		RandomChangeVelocity(newVelocity);
	}

	SetVelocity(newVelocity);
}

void ASnitch::RandomChangeVelocity(FVector& velocity) const
{
	const float RANDOM_CHANCE = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);

	if (chanceChangeDirection > RANDOM_CHANCE)
	{
		velocity = UQuidditchHelper::GetRandomVelocity() * maxSpeed;
	}
}
