// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuidditchHelper.generated.h"

/**
 * 
 */
UCLASS()
class QUIDDITCH_API UQuidditchHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Quidditch Helper")
	static FVector GetRandomVelocity();

	UFUNCTION(BlueprintCallable, Category = "Quidditch Helper")
	static FVector GetTargetVelocity(const FVector& currentVelocity, const float maxSpeed);

	UFUNCTION(BlueprintCallable, Category = "Quidditch Helper")
	static FVector InterpolateToTargetVelocity(const AActor* actor, const float maxSpeed, const float interpSpeed);

	UFUNCTION(BlueprintCallable, Category = "Quidditch Helper")
	static FVector EaseInVelocityToTargetDirection(FVector& currentVelocity, const FVector& targetDirection, const float maxSpeed, const float ALPHA = 0.2f);

	UFUNCTION(BlueprintCallable, Category = "Quidditch Helper")
	static bool IsPhysicalComponent(const class UActorComponent* component);

	UFUNCTION(BlueprintCallable, Category = "Quidditch Helper")
	static bool IsFirstNear(const FVector& first, const FVector& second);


private:

	static void CheckAndCorrectVelocity(FVector& velocity, const float maxSpeed);
};
