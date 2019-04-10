// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControlledCharacter.h"
#include "BaseController.h"
#include "MotionControllerComponent.h"
#include "QuidditchPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class QUIDDITCH_API AQuidditchPlayerController : public APlayerController, public IBaseController
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Controller settings")
	FName leftHandTag;

	UPROPERTY(EditDefaultsOnly, Category = "Controller settings")
	FName rightHandTag;

	FTimerHandle updateTimer;

	IControlledCharacter* player;

	UActorComponent* leftHand;
	UActorComponent* rightHand;

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void InitControlledPlayer();
	void InitMotionControllers();

	bool HasAnyMotionController();

	TArray<UActorComponent*> GetAllMotionControllers();

	bool CanAssignToLeftHand(UActorComponent* currentComponent);
	bool CanAssignToRightHand(UActorComponent* currentComponent);
	bool CanAssignToHand(UActorComponent* currentComponent, UActorComponent* hand, FName handTag);

	UFUNCTION()
	void UpdateRotation(const float DELTA_SECONDS);

	void SetupInputComponent() override;
	void Grab() override;
	
	UFUNCTION()
	void MoveForward(float value);

};
