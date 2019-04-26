// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Target.h"
#include "Hittable.h"
#include "GoalMaker.h"
#include "ControlledCharacter.h"
#include "Quidditch.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class QUIDDITCH_API ABaseCharacter : public ACharacter, public ITarget, public IHittable, public IGoalMaker, public IControlledCharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();

	//ITarget interface
	virtual bool CanBeAttacked() override;
	virtual FVector GetLocation() override;

	//IHittable interface
	virtual void AddDamage(FVector impulse) override;

	//IGoalMaker interface
	virtual ETeamSide GetTeam() override;

	//IControlledCharacter interface
	float GetSpeed() override;
	FVector GetDirection() override;
	void AddInput(const FVector DIRECTION, const float VALUE) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character settings")
	float maxSpeed;

	UCharacterMovementComponent* movementComponent;
	
	UPROPERTY(EditAnywhere, Category = "Character settings")
	ETeamSide team;

	UPROPERTY(EditAnywhere, Category = "Character settings")
	ERoleInTeam roleInTeam;

	virtual void BeginPlay() override;

private:

	void FindMovementComponent();
	void InitFlyingSpeed();
};
