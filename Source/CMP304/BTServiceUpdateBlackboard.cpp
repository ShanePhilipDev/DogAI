// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceUpdateBlackboard.h"
#include "Dog.h"
#include "DogAIController.h"
#include "DogTreat.h"
#include "DogBall.h"
#include "CMP304Character.h"

UBTServiceUpdateBlackboard::UBTServiceUpdateBlackboard()
{
	// The blackboard will be updated by this node every 0.1s.
	Interval = 0.1;
	RandomDeviation = 0;
}

void UBTServiceUpdateBlackboard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Dog shouldn't follow player to start off with.
	bFollowPlayer = false;

	// Get AI controller.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	// Get dog.
	ADog* Dog = NULL;
	if (AIController)
	{
		Dog = Cast<ADog>(AIController->GetPawn());
	}

	if (Dog)
	{
		// Get dog ball.
		ADogBall* DogBall = Cast<ADogBall>(UGameplayStatics::GetActorOfClass(GetWorld(), ADogBall::StaticClass()));

		// If dog ball exists...
		if (DogBall)
		{
			// Set dog ball pointer in the blackboard.
			AIController->GetBlackboardComponent()->SetValueAsObject(Ball.SelectedKeyName, DogBall);

			// If the dog has already fetched the ball...
			if (Dog->bHasFetchedBall)
			{
				// Set bool in blackboard so that the dog does not chase the ball anymore.
				AIController->GetBlackboardComponent()->SetValueAsBool(ShouldChaseBall.SelectedKeyName, false);

				// If the dog is carrying the ball, it should trot. Otherwise it should walk.
				if (Dog->GetCarryingBall())
				{
					Dog->GetCharacterMovement()->MaxWalkSpeed = Dog->GetTrotSpeed();
				}
				else
				{
					Dog->GetCharacterMovement()->MaxWalkSpeed = Dog->GetWalkSpeed();
				}
			}
			else
			{
				// If the dog hasn't fetched the ball, set the bool in the blackboard for the dog to chase the ball.
				AIController->GetBlackboardComponent()->SetValueAsBool(ShouldChaseBall.SelectedKeyName, true);

				// Dog should sprint.
				Dog->GetCharacterMovement()->MaxWalkSpeed = Dog->GetSprintSpeed();
			}
		}

		// Get dog treat.
		ADogTreat* DogTreat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(GetWorld(), ADogTreat::StaticClass()));

		// If dog treat exists...
		if (DogTreat)
		{
			// Set the eat treat blackboard bool value.
			AIController->GetBlackboardComponent()->SetValueAsBool(CanEatTreat.SelectedKeyName, true);

			// Set pointer to the treat in the blackboard.
			AIController->GetBlackboardComponent()->SetValueAsObject(Treat.SelectedKeyName, DogTreat);

			// The dog will trot towards the treat.
			Dog->GetCharacterMovement()->MaxWalkSpeed = Dog->GetTrotSpeed();
		}
		else
		{
			// If the dog treat doesn't exist, 
			AIController->GetBlackboardComponent()->SetValueAsBool(CanEatTreat.SelectedKeyName, false);
		}

		// Get the player.
		ACMP304Character* Player = Cast<ACMP304Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		// If the player exists...
		if (Player)
		{

			// Based on the player's selected tool and whether they possess the ball/treat, tell the blackboard if the player is holding the ball or treat.
			// ***** //
			switch (Player->Tool)
			{
			case ToolbarSelection::Nothing:
				bPlayerHasBall = false;
				bPlayerHasTreat = false;
				break;
			case ToolbarSelection::Ball:
				bPlayerHasBall = Player->bHasBall;
				bPlayerHasTreat = false;
				break;
			case::ToolbarSelection::Treat:
				bPlayerHasBall = false;
				bPlayerHasTreat = Player->bHasTreat;
				break;
			}

			AIController->GetBlackboardComponent()->SetValueAsBool(PlayerIsHoldingBall.SelectedKeyName, bPlayerHasBall);
			AIController->GetBlackboardComponent()->SetValueAsBool(PlayerIsHoldingTreat.SelectedKeyName, bPlayerHasTreat);
			// ***** //

			// If the player is holding the ball or treat, the dog should follow them.
			if (bPlayerHasBall)
			{
				bFollowPlayer = true;
			}
			else if (bPlayerHasTreat)
			{
				bFollowPlayer = true;
			}
		}

		// If the dog's hunger meter is below 15%, set the isHungry value in the blackboard to true and set the dog to follow the player.
		if (Dog->GetHungerMeter() < 0.15)
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(IsHungry.SelectedKeyName, true);
			bFollowPlayer = true;
		}
		else // Otherwise the dog is not hungry, and the blackboard value is false.
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(IsHungry.SelectedKeyName, false);
		}
	}

	// Set the blackboard value for following the player.
	AIController->GetBlackboardComponent()->SetValueAsBool(ShouldFollowPlayer.SelectedKeyName, bFollowPlayer);
}
