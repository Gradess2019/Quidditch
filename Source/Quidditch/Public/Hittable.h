// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHittable : public UInterface
{
	GENERATED_BODY()
};

/**
 * The interface of an object that can be hit by a bat
 */
class QUIDDITCH_API IHittable
{
	GENERATED_BODY()

public:

	virtual void AddDamage(FVector impulse) = 0;

};
