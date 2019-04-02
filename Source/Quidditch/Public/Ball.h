// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hittable.h"
#include "Components/ActorComponent.h"
#include "Ball.generated.h"

UCLASS(Abstract)
class QUIDDITCH_API ABall : public AActor, public IHittable
{
	GENERATED_BODY()

public:

	void AddDamage(FVector impulse) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Physical settings")
	float maxSpeed;
	
	/** Use to find a physical body */
	UPROPERTY(EditDefaultsOnly, Category = "Physical settings")
	FName physicalComponentTag;

	/** Use to automatically find a flight zone */
	UPROPERTY(EditDefaultsOnly, Category = "Flight zone settings")
	FName flightZoneTag;

	/** Specific flight zone */
	UPROPERTY(EditInstanceOnly, Category = "Flight zone settings")
	AActor* flightZone;

	FTimerHandle moveUpdateTimer;

	UPrimitiveComponent* physicalComponent;

	virtual void BeginPlay();
	virtual void Move();
	
	UFUNCTION()
	void OnLeaveFlightZone(AActor* otherActor);

	TArray<UActorComponent*> GetActorComponents();
	TArray<UActorComponent*> GetActorComponents(FName tag);
	
private:

	void FindPhysicalComponent();
	void FindFlightZone();

};
