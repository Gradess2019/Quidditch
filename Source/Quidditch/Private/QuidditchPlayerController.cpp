// Apache License. Copyright 2019 RTU IT Lab

#include "QuidditchPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

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
		for (UActorComponent* currentComponent : playerComponents)
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

bool AQuidditchPlayerController::HasAnyMotionController()
{
	return ensureMsgf(!(leftHandTag.IsNone() && rightHandTag.IsNone()), TEXT("Could not find any motion controller component in player"));
}

TArray<UActorComponent*> AQuidditchPlayerController::GetAllMotionControllers()
{
	return GetPawn()->GetComponentsByClass(UMotionControllerComponent::StaticClass());
}

bool AQuidditchPlayerController::CanAssignToLeftHand(UActorComponent* currentComponent)
{
	return CanAssignToHand(currentComponent, leftHand, leftHandTag);
}

bool AQuidditchPlayerController::CanAssignToHand(UActorComponent* currentComponent, UActorComponent* hand, FName handTag)
{
	return hand == nullptr && currentComponent->ComponentHasTag(handTag);
}

bool AQuidditchPlayerController::CanAssignToRightHand(UActorComponent* currentComponent)
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

}
