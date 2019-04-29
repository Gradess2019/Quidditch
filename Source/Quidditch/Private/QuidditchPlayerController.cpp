// Apache License. Copyright 2019 RTU IT Lab

#include "QuidditchPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttachableObject.h"
#include "Engine/Engine.h"

void AQuidditchPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitControlledPlayer();
	InitMotionControllers();
	InitChair();
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

void AQuidditchPlayerController::InitChair()
{
	bool connected = false;
	chair = UChairControl::SerialPort(connected, 8, this);
	chair->StartSending();
}

void AQuidditchPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateRotation(DeltaSeconds);
}

void AQuidditchPlayerController::UpdateRotation(const float DELTA_SECONDS)
{
	const FRotator PLAYER_ROTATION = GetPawn()->GetActorRotation();
	const FRotator TARGET_ROTATION = GetLeftHandRotation();
	const float INTERP_SPEED = 0.5f;
	const FRotator NEW_ROTATION = UKismetMathLibrary::RInterpTo(PLAYER_ROTATION, TARGET_ROTATION, DELTA_SECONDS, INTERP_SPEED);
	GetPawn()->SetActorRotation(NEW_ROTATION);

	const FVector RIGHT_VECTOR = GetPawn()->GetActorRightVector();
	const FVector FORWARD_VECTOR = GetPawn()->GetActorForwardVector();

	const float RIGHT_DOT_RESULT = UKismetMathLibrary::Dot_VectorVector(RIGHT_VECTOR, FVector::UpVector);
	const float RIGHT_COSINE = UKismetMathLibrary::DegAcos(RIGHT_DOT_RESULT) - 90.f;

	const float FORWARD_DOT_RESULT = UKismetMathLibrary::Dot_VectorVector(FORWARD_VECTOR, FVector::UpVector);
	const float FORWARD_COSINE = UKismetMathLibrary::DegAcos(FORWARD_DOT_RESULT) - 90.f;

	const float SQRT_RESULT = UKismetMathLibrary::Sqrt(RIGHT_COSINE * RIGHT_COSINE + FORWARD_COSINE * FORWARD_COSINE);

	const float CLAMP_RESULT = UKismetMathLibrary::FClamp(SQRT_RESULT, 0.f, 14.f);

	const float ROLL = (RIGHT_COSINE / SQRT_RESULT) * CLAMP_RESULT * (-1.f);
	const float PITCH = (FORWARD_COSINE / SQRT_RESULT) * CLAMP_RESULT * (-1.f);


	chair->Control(ROLL, PITCH);

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

