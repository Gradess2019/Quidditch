// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Goal.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UGoal : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface of an object which can invoke OnGoal event
 */
class QUIDDITCH_API IGoal
{
	GENERATED_BODY()

public:

	virtual int GetPoints() = 0;
	virtual bool ShouldFinishGame() = 0;

};
