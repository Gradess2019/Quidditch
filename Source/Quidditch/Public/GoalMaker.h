// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Quidditch.h"
#include "GoalMaker.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGoalMaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface of an object that hits IGoal objects
 */
class QUIDDITCH_API IGoalMaker
{
	GENERATED_BODY()

public:

	virtual	ETeamSide GetTeam() = 0;

};
