// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskSelectWanderPosition.h"
#include "Dog.h"
#include "DogAIController.h"
#include "NavigationSystem.h"
#include "Engine/World.h"


UBTTaskSelectWanderPosition::UBTTaskSelectWanderPosition()
{
	// Set wander radius.
	WanderRadius = 750;
}

// Execute the node.
EBTNodeResult::Type UBTTaskSelectWanderPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller, if it doesn't exist the task fails.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get the dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If the dog exists.
	if (Dog)
	{
		// Set dog's movement speed to walk speed.
		Dog->GetCharacterMovement()->MaxWalkSpeed = Dog->GetWalkSpeed();

		// Position to move to.
		FNavLocation Position;

		// Get the navigation system.
		UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		
		// Try to find a point to move to. If a point is found, set the blackboard value as that position. Otherwise set the point to the dog's current location.
		if (NavSystem->GetRandomReachablePointInRadius(Dog->GetActorLocation(), WanderRadius, Position))
		{
			AIController->GetBlackboardComponent()->SetValueAsVector(WanderPosition.SelectedKeyName, Position);
		}
		else
		{
			AIController->GetBlackboardComponent()->SetValueAsVector(WanderPosition.SelectedKeyName, Dog->GetActorLocation());
		}
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
