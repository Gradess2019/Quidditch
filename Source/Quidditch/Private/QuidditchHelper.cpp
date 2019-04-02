// Fill out your copyright notice in the Description page of Project Settings.

#include "QuidditchHelper.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

FVector UQuidditchHelper::GetRandomVelocity(float speed)
{
	return UKismetMathLibrary::RandomUnitVector() * speed;
}

FVector UQuidditchHelper::GetTargetVelocity(const FVector& currentVelocity, const float maxSpeed)
{
	FVector targetVelocity = FVector(maxSpeed);
	
	targetVelocity.X *= UKismetMathLibrary::SignOfFloat(currentVelocity.X);
	targetVelocity.Y *= UKismetMathLibrary::SignOfFloat(currentVelocity.Y);
	targetVelocity.Z *= UKismetMathLibrary::SignOfFloat(currentVelocity.Z);

	return targetVelocity;
}

FVector UQuidditchHelper::InterpolateToTargetVelocity(const AActor* actor, const float maxSpeed, const float interpSpeed)
{
	FVector currentVelocity = actor->GetVelocity();
	CheckAndCorrectVelocity(currentVelocity, maxSpeed);
	FVector targetVelocity = UQuidditchHelper::GetTargetVelocity(currentVelocity, maxSpeed);
	const float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(actor);

	FVector newVelocity = UKismetMathLibrary::VInterpTo_Constant(currentVelocity, targetVelocity, deltaTime, interpSpeed);
	return newVelocity;
}

void UQuidditchHelper::CheckAndCorrectVelocity(FVector& velocity, const float maxSpeed)
{
	if (velocity.IsZero())
	{
		velocity = GetRandomVelocity(maxSpeed);
	}
}

FVector UQuidditchHelper::EaseInVelocityToTargetDirection(FVector& currentVelocity, const FVector& targetDirection, const float maxSpeed, const float ALPHA)
{
	CheckAndCorrectVelocity(currentVelocity, maxSpeed);

	FVector newVelocity = UKismetMathLibrary::VEase(currentVelocity, targetDirection, ALPHA, EEasingFunc::EaseIn);
	return newVelocity;

}

bool UQuidditchHelper::IsPhysicalComponent(const UActorComponent* component)
{
	return component->IsA(UPrimitiveComponent::StaticClass());
}

bool UQuidditchHelper::IsFirstNear(const FVector& first, const FVector& second)
{
	return UKismetMathLibrary::VSize(first) < UKismetMathLibrary::VSize(second);
}