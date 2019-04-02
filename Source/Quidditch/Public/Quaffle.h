// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "AttachableObject.h"
#include "Goal.h"
#include "Quaffle.generated.h"

/**
 * 
 */
UCLASS()
class QUIDDITCH_API AQuaffle : public ABall, public IAttachableObject, public IGoal
{
	GENERATED_BODY()

public:

	virtual void Attach(USceneComponent* parentComponent) override;
	virtual void Detach() override;

	virtual int GetPoints() override;
	virtual bool ShouldFinishGame() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Goal interface settings")
	int points;

	UPROPERTY(EditDefaultsOnly, Category = "Goal interface settings")
	bool finishGame;

	FVector lastDeltaLocation;

	void Move() override;
	
	FVector GetNewVelocity();

	bool CorrectDirection(FVector& velocity);

	void SetVelocity(const FVector& velocity);
};
