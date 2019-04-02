// Apache License. Copyright 2019 RTU IT Lab

#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	speed = 100.f;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	FindMovementComponent();
}

void ABaseCharacter::FindMovementComponent()
{
	UPawnMovementComponent* movementComponent = GetMovementComponent();
	bool isCharacterMovementComponent = movementComponent->IsA(UCharacterMovementComponent::StaticClass());
	if (ensureMsgf(isCharacterMovementComponent, TEXT("Could not find UCharacterMovementComponent!")))
	{
		this->movementComponent = Cast<UCharacterMovementComponent>(movementComponent);
	}
}

bool ABaseCharacter::CanBeAttacked() 
{
	switch (roleInTeam)
	{
		case ERoleInTeam::Keeper:
		{
			return false;
		}

		default:
		{
			return true;
		}
	}
}

FVector ABaseCharacter::GetLocation()
{
	return GetActorLocation();
}

void ABaseCharacter::AddDamage(FVector impulse)
{
	const FVector resultImpulse = impulse / movementComponent->Mass;
	const bool changeVelocity = true;
	movementComponent->AddImpulse(resultImpulse, changeVelocity);
}

ETeamSide ABaseCharacter::GetTeam()
{
	return team;
}

float ABaseCharacter::GetSpeed()
{
	return speed;
}