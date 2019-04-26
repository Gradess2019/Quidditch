// Apache License. Copyright 2019 RTU IT Lab

#include "QuidditchPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttachableObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

void AQuidditchPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitControlledPlayer();
	InitMotionControllers();
}

void AQuidditchPlayerController::InitControlledPlayer()
{
	player = Cast<IControlledCharacter>(GetPawn());
}

void AQuidditchPlayerController::InitMotionControllers()
{
	if (HasAnyMotionController())
	{
		TArray<UActorComponent*> playerComponents = GetAllMotionControllers();
		for (UActorComponent* component : playerComponents)
		{
			UPrimitiveComponent* currentComponent = Cast<UPrimitiveComponent>(component);
			if (currentComponent)
			{
				if (CanAssignToLeftHand(currentComponent))
				{
					leftHand = currentComponent;
				}
				else if (CanAssignToRightHand(currentComponent))
				{
					rightHand = currentComponent;
				}
			}
		}
	}
}

bool AQuidditchPlayerController::HasAnyMotionController() const
{
	return ensureMsgf(!(leftHandTag.IsNone() && rightHandTag.IsNone()), TEXT("Could not find any motion controller component in player"));
}

TArray<UActorComponent*> AQuidditchPlayerController::GetAllMotionControllers() const
{
	return GetPawn()->GetComponentsByClass(UMotionControllerComponent::StaticClass());
}

bool AQuidditchPlayerController::CanAssignToLeftHand(UPrimitiveComponent* currentComponent) const
{
	return CanAssignToHand(currentComponent, leftHand, leftHandTag);
}

bool AQuidditchPlayerController::CanAssignToHand(UPrimitiveComponent* currentComponent, UActorComponent* hand, FName handTag) const
{
	return hand == nullptr && currentComponent->ComponentHasTag(handTag);
}

bool AQuidditchPlayerController::CanAssignToRightHand(UPrimitiveComponent* currentComponent) const
{
	return CanAssignToHand(currentComponent, rightHand, rightHandTag);
}

void AQuidditchPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateRotation(DeltaSeconds);
}

void AQuidditchPlayerController::UpdateRotation(const float DELTA_SECONDS)
{
	const FRotator playerRotation = GetPawn()->GetActorRotation();
	const FRotator targetRotation = GetLeftHandRotation();
	const float INTERP_SPEED = 0.5f;
	const FRotator newRotation = UKismetMathLibrary::RInterpTo(playerRotation, targetRotation, DELTA_SECONDS, INTERP_SPEED);
	GetPawn()->SetActorRotation(newRotation);
}

FRotator AQuidditchPlayerController::GetLeftHandRotation() const
{
	USceneComponent* sceneComponent = Cast<USceneComponent>(leftHand);
	return sceneComponent->GetComponentRotation();
}

void AQuidditchPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AQuidditchPlayerController::MoveForward);
	InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &AQuidditchPlayerController::Grab);
	InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &AQuidditchPlayerController::Ungrab);
}

void AQuidditchPlayerController::MoveForward(float value)
{
	if (value != 0.0)
	{
		const FVector DIRECTION = player->GetDirection();
		player->AddInput(DIRECTION, value);
	}
}

void AQuidditchPlayerController::Grab()
{
	GetOverlappedActor();
	if (attachableObject)
	{
		attachableObject->Attach(rightHand);
	}
}

void AQuidditchPlayerController::Ungrab()
{
	if (attachableObject)
	{
		attachableObject->Detach();
		attachableObject = nullptr;
	}
}

void AQuidditchPlayerController::GetOverlappedActor()
{
	TArray<AActor*> overlappedActors;

	UPrimitiveComponent* trigger = GetRightHandTrigger();
	trigger->GetOverlappingActors(overlappedActors);

	for (AActor* actor : overlappedActors)
	{
		IAttachableObject* attachableObject = Cast<IAttachableObject>(actor);
		if (attachableObject)
		{
			this->attachableObject = attachableObject;
			break;
		}
	}
}

UPrimitiveComponent* AQuidditchPlayerController::GetRightHandTrigger() const
{
	TArray<UActorComponent*> components = GetPawn()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), rightHandTriggerTag);
	return Cast<UPrimitiveComponent>(components[0]);
}

