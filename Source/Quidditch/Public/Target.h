// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Target.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface of an object which can be attacked by Bludger
 */
class QUIDDITCH_API ITarget
{
	GENERATED_BODY()

public:

	virtual bool CanBeAttacked() = 0;
	virtual FVector GetLocation() = 0;
};
