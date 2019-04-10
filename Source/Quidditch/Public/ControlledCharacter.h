// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Quidditch.h"
#include "ControlledCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControlledCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface of an object that is a Character
 */
class QUIDDITCH_API IControlledCharacter
{
	GENERATED_BODY()

public:

	virtual float GetSpeed() = 0;
	virtual FVector GetDirection() = 0;
	virtual void AddInput(const FVector DIRECTION, const float VALUE) = 0;
};
