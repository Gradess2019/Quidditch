// Apache License. Copyright 2019 RTU IT Lab

#include "Bludger.h"
#include "QuidditchHelper.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ABludger::BeginPlay()
{
	Super::BeginPlay();

	FindTargetTrigger();
	FindTarget();
}

void ABludger::FindTargetTrigger()
{
	TArray<UActorComponent*> components = GetActorComponents(triggerComponentTag);

	for (UActorComponent* currentComponent : components)
	{
		if (UQuidditchHelper::IsPhysicalComponent(currentComponent))
		{
			targetTrigger = Cast<UPrimitiveComponent>(currentComponent);
			break;
		}
	}

	ensureMsgf(targetTrigger, TEXT("Target trigger is null! \n Check your trigger component and its tag!"));
}

void ABludger::FindTarget()
{
	TArray<AActor*> overlappingActors;
	UGameplayStatics::GetAllActorsWithInterface(this, UTarget::StaticClass(), overlappingActors);

	AActor* nearestTarget = FindTargetInArray(overlappingActors);
	SetTarget(nearestTarget);
}

AActor* ABludger::FindTargetInArray(TArray<AActor*> actors)
{
	AActor* nearestTarget = nullptr;

	if (actors.Num() > 0)
	{
		for (AActor* currentActor : actors)
		{

			if (!nearestTarget)
			{
				nearestTarget = currentActor;
			}
			else
			{
				FVector distanceToNearest = nearestTarget->GetActorLocation() - GetActorLocation();
				FVector distanceToOther = currentActor->GetActorLocation() - GetActorLocation();
				if (UQuidditchHelper::IsFirstNear(distanceToOther, distanceToNearest))
				{
					nearestTarget = currentActor;
				}
			}
		}
	}

	return nearestTarget;
}

void ABludger::SetTarget(AActor* nearestTarget)
{
	if (nearestTarget)
	{
		target = Cast<ITarget>(nearestTarget);
	}
	else
	{
		target = nullptr;
	}
}

void ABludger::Move()
{
	if (target)
	{
		FVector direction = target->GetLocation() - GetActorLocation();
		direction.Normalize(0.f);
		direction *= maxSpeed;

		FVector velocity = GetVelocity();
		velocity = UQuidditchHelper::EaseInVelocityToTargetDirection(velocity, direction, maxSpeed);

		physicalComponent->SetPhysicsLinearVelocity(velocity);
	}
	else
	{
		FindTarget();
	}
}