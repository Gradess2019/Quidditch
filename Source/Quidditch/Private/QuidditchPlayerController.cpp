// Apache License. Copyright 2019 RTU IT Lab

#include "QuidditchPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttachableObject.h"
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

bool AQuidditchPlayerController::HasAnyMotionController()
{
	return ensureMsgf(!(leftHandTag.IsNone() && rightHandTag.IsNone()), TEXT("Could not find any motion controller component in player"));
}

TArray<UActorComponent*> AQuidditchPlayerController::GetAllMotionControllers()
{
	return GetPawn()->GetComponentsByClass(UMotionControllerComponent::StaticClass());
}

bool AQuidditchPlayerController::CanAssignToLeftHand(UPrimitiveComponent* currentComponent)
{
	return CanAssignToHand(currentComponent, leftHand, leftHandTag);
}

bool AQuidditchPlayerController::CanAssignToHand(UPrimitiveComponent* currentComponent, UActorComponent* hand, FName handTag)
{
	return hand == nullptr && currentComponent->ComponentHasTag(handTag);
}

bool AQuidditchPlayerController::CanAssignToRightHand(UPrimitiveComponent* currentComponent)
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
	FRotator playerRotation = GetPawn()->GetActorRotation();
	USceneComponent* scneComponent = Cast<USceneComponent>(leftHand);
	FRotator targetRotation = scneComponent->GetComponentRotation();
	const float INTERP_SPEED = 0.5f;
	FRotator newRotation = UKismetMathLibrary::RInterpTo(playerRotation, targetRotation, DELTA_SECONDS, INTERP_SPEED);
	GetPawn()->SetActorRotation(newRotation);
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
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("TRY TO GRAB"));
	GetOverlappedActor();
	if (attachableObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("ATTACH"));
		attachableObject->Attach(rightHand);
	}

}

void AQuidditchPlayerController::Ungrab()
{
	if (attachableObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, TEXT("DETACH"));
		attachableObject->Detach();
	}
}

void AQuidditchPlayerController::GetOverlappedActor()
{
	TArray<AActor*> overlappedActors;
	rightHand->GetOverlappingActors(overlappedActors);

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

