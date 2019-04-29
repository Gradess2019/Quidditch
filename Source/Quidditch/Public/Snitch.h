// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "Quaffle.h"
#include "Snitch.generated.h"

UCLASS()
class QUIDDITCH_API ASnitch : public AQuaffle
{
	GENERATED_BODY()
	

private:

	UPROPERTY(EditAnywhere, Category = "Physical settings")
	float chanceChangeDirection;

	void Move() override;
	void RandomChangeVelocity(FVector& velocity) const;
};
