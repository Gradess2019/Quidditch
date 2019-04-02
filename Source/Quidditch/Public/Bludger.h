// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "Target.h"
#include "Components/PrimitiveComponent.h"
#include "Bludger.generated.h"

/**
 * A ball attacks a ITarget
 */
UCLASS()
class QUIDDITCH_API ABludger : public ABall
{
	GENERATED_BODY()
	

private:

	ITarget* target;

	UPrimitiveComponent* targetTrigger;

	/** Use to find a trigger targets */
	UPROPERTY(EditDefaultsOnly, Category = "Physical settings")
	FName triggerComponentTag;

	FTimerHandle findTargetTimer;

	void BeginPlay() override;

	void FindTargetTrigger();
	void FindTarget();
	void SetTarget(AActor* nearestTarget);

	AActor* FindTargetInArray(TArray<AActor*> actors);

	void Move() override;
};
