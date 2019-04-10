// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBaseController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUIDDITCH_API IBaseController
{
	GENERATED_BODY()

public:

	virtual void Grab() = 0;
};
