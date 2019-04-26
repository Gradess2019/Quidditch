// Apache License. Copyright 2019 RTU IT Lab

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControlledCharacter.h"
#include "BaseController.h"
#include "MotionControllerComponent.h"
#include "AttachableObject.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Controller settings")
	FName rightHandTriggerTag;

	FTimerHandle updateTimer;

	IControlledCharacter* player;

	UPrimitiveComponent* leftHand;
	UPrimitiveComponent* rightHand;

	IAttachableObject* attachableObject;

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	FRotator GetLeftHandRotation() const;

	void InitControlledPlayer();
	void InitMotionControllers();

	bool HasAnyMotionController() const;

	TArray<UActorComponent*> GetAllMotionControllers() const;

	bool CanAssignToLeftHand(UPrimitiveComponent* currentComponent) const;
	bool CanAssignToRightHand(UPrimitiveComponent* currentComponent) const;
	bool CanAssignToHand(UPrimitiveComponent* currentComponent, UActorComponent* hand, FName handTag) const;

	UFUNCTION()
	void UpdateRotation(const float DELTA_SECONDS);

	void SetupInputComponent() override;
	void Grab() override;
	void Ungrab() override;

	void GetOverlappedActor();
	UPrimitiveComponent* GetRightHandTrigger() const;
	
	
	UFUNCTION()
	void MoveForward(float value);

};
