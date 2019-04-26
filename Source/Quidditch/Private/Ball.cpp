// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "QuidditchHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ABall::BeginPlay()
{
	Super::BeginPlay();

	if (physicalComponent == nullptr)
	{
		FindPhysicalComponent();
	}

	if (flightZone == nullptr)
	{
		FindFlightZone();
	}

	GetWorld()->GetTimerManager().SetTimer(movementUpdateTimer, this, &ABall::Move, 0.1f, true);
}

void ABall::FindPhysicalComponent()
{
	TArray<UActorComponent*> components = GetActorComponents();

	for (UActorComponent* currentComponent : components)
	{
		if (UQuidditchHelper::IsPhysicalComponent(currentComponent))
		{
			physicalComponent = Cast<UPrimitiveComponent>(currentComponent);
			break;
		}
	}

	ensureMsgf(physicalComponent, TEXT("Physical components is null! \n Check your physical component and its tag!"));
}

TArray<UActorComponent*> ABall::GetActorComponents()
{
	return GetActorComponents(physicalComponentTag);
}

TArray<UActorComponent*> ABall::GetActorComponents(FName tag)
{
	if (tag.IsNone())
	{
		return GetComponentsByClass(UActorComponent::StaticClass());
	}
	else
	{
		return GetComponentsByTag(UActorComponent::StaticClass(), tag);
	}
}

void ABall::FindFlightZone()
{
	TArray<AActor*> flightZones;
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), flightZoneTag, flightZones);

	if (ensureMsgf(flightZones.Num() > 0, TEXT("Could not find any flight zone!")))
	{
		flightZone = flightZones[0];
	}
}

void ABall::OnLeaveFlightZone(AActor* otherActor)
{
	Move();
}

void ABall::Move()
{
	unimplemented();
}

void ABall::AddDamage(FVector impulse)
{
	
}
