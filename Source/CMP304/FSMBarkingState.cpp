


// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMBarkingState.h"
#include "Dog.h"
#include "DogTreat.h"

UFSMBarkingState::UFSMBarkingState()
{
	// Set state name and default values.
	StateName = "Barking";
	bFinishedBarking = false;
	StopBarkingTime = 0.8;
}

void UFSMBarkingState::Run(float DeltaSeconds)
{
	// Cast to dog classs.
	ADog* DogCast = Cast<ADog>(Dog);

	if (DogCast)
	{
		// Move the dog to the player. When it reaches the player, start barking.
		if (AIController->MoveToActor(Player, 100) == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			DogCast->SetBarking(true);
		}
	}

	// Get dog treat.
	ADogTreat* Treat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(World, ADogTreat::StaticClass()));

	// If the treat exists, stop barking. Start the timer to wait for the animation to finish. Once the timer has finished, the dog can change state.
	if (Treat)
	{
		DogCast->SetBarking(false);
		if (!World->GetTimerManager().IsTimerActive(StopBarkingTimer))
		{
			World->GetTimerManager().SetTimer(StopBarkingTimer, this, &UFSMBarkingState::StopBarking, StopBarkingTime);
		}
	}
}

bool UFSMBarkingState::SwitchState(FName& NewState)
{
	// This state only switches when the dog has finished barking (when a treat is ready to be eaten).
	if (bFinishedBarking)
	{
		// The new state is eating.
		bFinishedBarking = false;
		NewState = "Eating";
		return true;
	}
	return false;
}

void UFSMBarkingState::StopBarking()
{
	// Set finished barking to true.
	bFinishedBarking = true;
}
