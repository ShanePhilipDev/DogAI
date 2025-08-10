// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMReturningBallState.h"
#include "Dog.h"

UFSMReturningBallState::UFSMReturningBallState()
{
	// Set state name and timer length.
	StateName = "ReturningBall";
	WaitTime = 0.2;
}

UFSMReturningBallState::~UFSMReturningBallState()
{
}

void UFSMReturningBallState::Run(float DeltaSeconds)
{
	// Move towards the player. Once the player has been reached, start the timer.
	if (AIController->MoveToActor(Player, 100) == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if (!World->GetTimerManager().IsTimerActive(WaitTimer))
		{
			World->GetTimerManager().SetTimer(WaitTimer, this, &UFSMReturningBallState::DropBall, WaitTime);
		}
	}
	
}

bool UFSMReturningBallState::SwitchState(FName& NewState)
{
	// Cast to dog
	ADog* DogCast = Cast<ADog>(Dog);

	// If dog is no longer carrying the ball, adjust speed and switch to wander state.
	if (!DogCast->GetCarryingBall())
	{
		DogCast->GetCharacterMovement()->MaxWalkSpeed = DogCast->GetWalkSpeed();
		NewState = "Wander";
		return true;
	}
	
	return false;
}

// Once timer is finished, this will execute.
void UFSMReturningBallState::DropBall()
{
	// Cast to dog and get attached actors.
	ADog* DogCast = Cast<ADog>(Dog);
	TArray<AActor*> AttachedActors;
	Dog->GetAttachedActors(AttachedActors);

	// If the dog doesn't have the ball attached (if the player has grabbed it before he could drop it), set carrying ball to false.
	if (!AttachedActors.IsValidIndex(0))
	{
		DogCast->SetCarryingBall(false);
	}
	else
	{
		// Cast to dog ball.
		// Should only be 1 attached actor, so access element 0.
		DogBall = Cast<ADogBall>(AttachedActors[0]);

		// If the ball is attached, detach it and set it to simulate physics. Set carrying ball to false.
		if (DogBall)
		{
			DogBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			DogBall->Mesh->SetSimulatePhysics(true);
			DogCast->SetCarryingBall(false);
		}
		else
		{
			// Set carrying ball to false if dog ball is not set.
			DogCast->SetCarryingBall(false);
		}
	}
}
