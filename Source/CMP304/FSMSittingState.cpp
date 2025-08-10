// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMSittingState.h"
#include "Dog.h"
#include "CMP304Character.h"
#include "DogTreat.h"

UFSMSittingState::UFSMSittingState()
{
	// Set state name and default values.
	StateName = "Sitting";
	StandUpTime = 1;
	bIsStanding = true;
}

// From Epic Games' BTTask_RotateToFaceBBEntry class, for calculating if dog is facing player or not.
float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
{
	return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
		? 1.f
		: VectorA.CosineAngle2D(VectorB);
}

void UFSMSittingState::Run(float DeltaSeconds)
{
	// Cast dog and player.
	ADog* DogCast = Cast<ADog>(Dog);
	ACMP304Character* PlayerCast = Cast<ACMP304Character>(Player);

	// Look at player.
	AIController->SetFocus(Player);
	
	// Calculate if the dog is facing the player, with a precision of 10 degrees.
	float AngleDif = CalculateAngleDifferenceDot(Dog->GetActorForwardVector(), (Player->GetActorLocation() - Dog->GetActorLocation()));
	float DotPrecision = FMath::Cos(FMath::DegreesToRadians(10));
	
	// Move towards the player. If the player is reached and the dog is facing the player, the dog will sit.
	if (AIController->MoveToActor(Player, 100) == EPathFollowingRequestResult::AlreadyAtGoal && AngleDif >= DotPrecision)
	{
		DogCast->SetSitting(true);
		bIsStanding = false;
	}
	
	// Get dog treat.
	ADogTreat* Treat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(World, ADogTreat::StaticClass()));

	// If the treat exists or the player has the treat but is not holding it...
	if (Treat || (PlayerCast->bHasTreat && PlayerCast->Tool != ToolbarSelection::Treat))
	{
		// Focus on the treat.
		if (Treat)
		{
			AIController->SetFocus(Treat);
		}
		
		// Stop sitting.
		DogCast->SetSitting(false);

		// Set timer to delay state change until after finished standing up.
		if (!World->GetTimerManager().IsTimerActive(StandUpTimer))
		{
			World->GetTimerManager().SetTimer(StandUpTimer, this, &UFSMSittingState::StandUp, StandUpTime);
		}
		
	}
}

bool UFSMSittingState::SwitchState(FName& NewState)
{
	// Cast dog and player.
	ADog* DogCast = Cast<ADog>(Dog);
	ACMP304Character* PlayerCast = Cast<ACMP304Character>(Player);

	// Get treat.
	ADogTreat* Treat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(World, ADogTreat::StaticClass()));

	// If dog is hungry, switch to barking state.
	if (DogCast->GetHungerMeter() < 0.15)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		NewState = "Barking";
		DogCast->SetSitting(false);
		return true;
	}

	// If player is no longer holding treat, switch to wander state.
	if (PlayerCast->bHasTreat && (PlayerCast->Tool != ToolbarSelection::Treat))
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		bIsStanding = true;
		DogCast->SetSitting(false);
		NewState = "Wander";
		return true;
	}

	// If treat exists and finished standing up, switch to eating state.
	if (Treat && bIsStanding)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		bIsStanding = true;
		DogCast->SetSitting(false);
		NewState = "Eating";
		return true;
	}

	return false;
}

// Executes once timer is finished, sets standing to true.
void UFSMSittingState::StandUp()
{
	bIsStanding = true;
}


