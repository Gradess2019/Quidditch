// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Target.h"
#include "Hittable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class QUIDDITCH_API ABaseCharacter : public ACharacter, public ITarget, public IHittable
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	//ITarget interface
	virtual bool CanBeAttacked() override;
	virtual FVector GetLocation() override;

	//IHittable interface
	virtual void AddDamage(FVector impulse) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Character settings")
	float speed;

	UCharacterMovementComponent* movementComponent;

	//Uncomment it when ETeamSide enum will exists
	//TEnumAsByte<ETeamSide> team;

	//Uncomment it when ETeamSide enum will exists
	//TEnumAsByte<ERoleInTeam> role;

	virtual void BeginPlay() override;

private:

	void FindMovementComponent();
};
