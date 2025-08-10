// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMEatingState.h"
#include "Dog.h"

UFSMEatingState::UFSMEatingState()
{
	// Set state name and default values.
	StateName = "Eating";
	DogTreat = nullptr;
	bFinishedEating = false;
	EatTime = 2.5;
}

void UFSMEatingState::Run(float DeltaSeconds)
{
	// Cast to dog class.
	ADog* CastDog = Cast<ADog>(Dog);

	// Get dog treat if pointer doesn't already have a value.
	if (!DogTreat)
	{
		DogTreat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(World, ADogTreat::StaticClass()));
	}
	
	// If treat exists...
	if (DogTreat)
	{
		// Trot towards the treat.
		CastDog->GetCharacterMovement()->MaxWalkSpeed = CastDog->GetTrotSpeed();

		// The dog will move towards the treat. Once it has reached the treat, it will start eating and destroy the treat.
		if (AIController->MoveToActor(DogTreat) == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			CastDog->SetEating(true);
			DogTreat->Destroy();
			DogTreat = nullptr;

			// A timer is started to delay state change until after the animation is finished.
			World->GetTimerManager().SetTimer(EatTimer, this, &UFSMEatingState::FinishEating, EatTime);
		}
	}
}

bool UFSMEatingState::SwitchState(FName& NewState)
{
	// Cast to dog class and set dog's speed to walking speed.
	ADog* CastDog = Cast<ADog>(Dog);
	CastDog->GetCharacterMovement()->MaxWalkSpeed = CastDog->GetWalkSpeed();

	// Switch state to wandering once finished eating.
	if (bFinishedEating)
	{
		bFinishedEating = false;
		DogTreat = nullptr;
		NewState = "Wander";
		return true;
	}

	return false;
}

void UFSMEatingState::FinishEating()
{
	// Cast to dog class.
	ADog* CastDog = Cast<ADog>(Dog);

	// Finish eating.
	bFinishedEating = true;

	// Reset dog's hunger.
	CastDog->SetHungerMeter(1);
}
