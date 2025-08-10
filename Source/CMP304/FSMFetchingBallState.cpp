// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMFetchingBallState.h"
#include "Dog.h"
#include "CMP304Character.h"

UFSMFetchingBallState::UFSMFetchingBallState()
{
	// Set state name and dog ball to nullptr.
	StateName = "FetchingBall";
	DogBall = nullptr;
}


UFSMFetchingBallState::~UFSMFetchingBallState()
{
}

void UFSMFetchingBallState::Run(float DeltaSeconds)
{
	// Cast to dog.
	ADog* DogCast = Cast<ADog>(Dog);

	// If the dog ball pointer hasn't been set i.e. the player is holding the ball but hasn't released it...
	if (!DogBall)
	{
		// Try to set dog ball to check if player has released it.
		DogBall = Cast<ADogBall>(UGameplayStatics::GetActorOfClass(World, ADogBall::StaticClass()));

		// Set dog to walk towards and face the player.
		DogCast->GetCharacterMovement()->MaxWalkSpeed = DogCast->GetWalkSpeed();
		AIController->MoveToActor(Player, 100);
		AIController->SetFocus(Player);
	}

	// When the dog ball pointer is set...
	if (DogBall)
	{
		if (DogCast)
		{
			// The dog will focus on the ball.
			AIController->SetFocus(DogBall);

			// Set dog speed to sprint.
			DogCast->GetCharacterMovement()->MaxWalkSpeed = DogCast->GetSprintSpeed();

			// Move directly to the ball. When the ball is reached, pick it up.
			if (AIController->MoveToActor(DogBall, -1, true, false) == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				// Stop simulating physics on the ball and attach it to the dog.
				DogBall->Mesh->SetSimulatePhysics(false);
				DogBall->AttachToComponent(DogCast->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "mouthSocket");

				// Set dog variables and speed.
				DogCast->bIsPickingUpBall = true;
				DogCast->bHasFetchedBall = true;
				DogCast->SetCarryingBall(true);
				DogCast->GetCharacterMovement()->MaxWalkSpeed = DogCast->GetTrotSpeed();
			}
		}
	}
	
}

bool UFSMFetchingBallState::SwitchState(FName& NewState)
{
	// Cast to player and dog.
	ACMP304Character* PlayerCast = Cast<ACMP304Character>(Player);
	ADog* DogCast = Cast<ADog>(Dog);
	
	// Clear AI's focused object.
	AIController->ClearFocus(EAIFocusPriority::Gameplay);

	// If dog is hungry, switch to barking state.
	if (DogCast->GetHungerMeter() < 0.15)
	{
		DogBall = nullptr;
		NewState = "Barking";
		return true;
	}

	if (DogCast)
	{
		// If dog is carrying ball, change speed to trot and switch to return ball state.
		if (DogCast->GetCarryingBall())
		{
			DogBall = nullptr;
			DogCast->GetCharacterMovement()->MaxWalkSpeed = DogCast->GetTrotSpeed();
			NewState = "ReturningBall";
			return true;
		}
	}

	// If player is holding the treat, change speed to trot and set state as sitting.
	if (PlayerCast->bHasTreat && PlayerCast->Tool == ToolbarSelection::Treat)
	{
		DogCast->GetCharacterMovement()->MaxWalkSpeed = DogCast->GetTrotSpeed();
		DogBall = nullptr;
		NewState = "Sitting";
		return true;
	}

	// If ball has been returned and the player is not holding it, switch to wander state.
	if (PlayerCast->bHasBall && PlayerCast->Tool != ToolbarSelection::Ball)
	{
		DogBall = nullptr;
		NewState = "Wander";
		return true;
	}
	
	return false;
}
