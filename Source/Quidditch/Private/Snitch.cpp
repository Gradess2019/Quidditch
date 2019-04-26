// Apache License. Copyright 2019 RTU IT Lab

#include "Snitch.h"
#include "QuidditchHelper.h"
#include "Kismet/KismetMathLibrary.h"

void ASnitch::Move()
{
	FVector newVelocity = GetNewVelocity();

	bool wasCorrected = CorrectDirection(newVelocity);

	if (!wasCorrected)
	{
		RandomChangeVelocity(newVelocity);
	}

	SetVelocity(newVelocity);
}

void ASnitch::RandomChangeVelocity(FVector& velocity)
{
	float randomChance = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);

	if (chanceChangeDirection > randomChance)
	{
		velocity = UQuidditchHelper::GetRandomVelocity();
	}
}