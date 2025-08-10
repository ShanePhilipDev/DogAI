// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMWanderState.h"
#include "Kismet/KismetMathLibrary.h"
#include "DogBall.h"
#include "Dog.h"
#include "DogTreat.h"
#include "CMP304Character.h"

#include <time.h>

UFSMWanderState::UFSMWanderState()
{
	// Set state name, set default values, and seed rand with time.
	StateName = "Wander";
	WanderRadius = 750;
	bFirstRun = true;
	WaitTime = 3;
	srand(time(NULL));
}

UFSMWanderState::~UFSMWanderState()
{
}

void UFSMWanderState::Run(float DeltaSeconds)
{
	// This is true when switching to this state. Sets wander position to dog's current location, so it starts waiting.
	if (bFirstRun)
	{
		WanderPosition = Dog->GetActorLocation();
		bFirstRun = false;
	}

	// Move to the desired location, and start the timer when reached.
	if (AIController->MoveToLocation(WanderPosition) == EPathFollowingRequestResult::AlreadyAtGoal)
	{
	
		if (!World->GetTimerManager().IsTimerActive(IdleTimer))
		{
			World->GetTimerManager().SetTimer(IdleTimer, this, &UFSMWanderState::FindNewPosition, WaitTime);
		}
		
	}
}

bool UFSMWanderState::SwitchState(FName& NewState)
{
	// Get ball.
	ADogBall* Ball = Cast<ADogBall>(UGameplayStatics::GetActorOfClass(World, ADogBall::StaticClass()));
	ADogTreat* Treat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(World, ADogTreat::StaticClass()));

	// Cast dog and player.
	ADog* DogCast = Cast<ADog>(Dog);
	ACMP304Character* PlayerCast = Cast<ACMP304Character>(Player);

	// If treat exists, switch to eating state.
	if (Treat)
	{
		bFirstRun = true;
		NewState = "Eating";
		return true;
	}

	// If dog is hungry, switch to barking state.
	if (DogCast->GetHungerMeter() < 0.15)
	{
		bFirstRun = true;
		NewState = "Barking";
		return true;
	}

	// If player is holding the ball or the ball exists and the dog hasn't already fetched it, start fetching the ball.
	if ((Ball && !DogCast->bHasFetchedBall) || (PlayerCast->bHasBall && PlayerCast->Tool == ToolbarSelection::Ball))
	{
		bFirstRun = true;
		NewState = "FetchingBall";
		return true;
	}

	// If the player is holding the treat, start sitting.
	if (PlayerCast->bHasTreat && PlayerCast->Tool == ToolbarSelection::Treat)
	{
		bFirstRun = true;
		NewState = "Sitting";
		return true;
	}

	return false;
}

void UFSMWanderState::FindNewPosition()
{
	// New position.
	FNavLocation Position;

	// Get nav system.
	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(World->GetNavigationSystem());

	// Wander position = dog's current location. If next function fails, the dog will use this position.
	WanderPosition = Dog->GetActorLocation();

	if (NavSystem)
	{
		// Find new position to move to. If valid, set the wander position.
		if (NavSystem->GetRandomReachablePointInRadius(Dog->GetActorLocation(), WanderRadius, Position))
		{
			WanderPosition = Position;
		}
	}
	
	// Randomise the wait time.
	WaitTime = 1 + (rand() % 4);
}
