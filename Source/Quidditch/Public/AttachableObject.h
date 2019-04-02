// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/SceneComponent.h"
#include "AttachableObject.generated.h"

UINTERFACE(MinimalAPI)
class UAttachableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface of an object which can be attach to Character's hand
 */
class QUIDDITCH_API IAttachableObject
{
	GENERATED_BODY()

public:

	virtual void Attach(USceneComponent* parentComponent) = 0;
	virtual void Detach() = 0;

protected:

	bool isAttach;

};
