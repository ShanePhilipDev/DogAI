// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskDropBall.h"
#include "Dog.h"
#include "DogAIController.h"
#include "DogBall.h"

// Execute the node.
EBTNodeResult::Type UBTTaskDropBall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller, if it doesn't exist the task fails.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get the dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If the dog exists...
	if (Dog)
	{
		// Get the dog's attached actors.
		TArray<AActor*> AttachedActors;
		Dog->GetAttachedActors(AttachedActors);

		// Pointer to the ball.
		ADogBall* Ball;

		// If the dog has no attached actors, the player has taken the ball from the dog's mouth.
		if (!AttachedActors.IsValidIndex(0))
		{
			// Set the blackboard value of HasBall to false.
			AIController->GetBlackboardComponent()->SetValueAsBool(HasBall.SelectedKeyName, false);

			// Set dog's carrying ball variable to false.
			Dog->SetCarryingBall(false);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			// Set pointer to the ball if the dog does have attached actors.
			Ball = Cast<ADogBall>(AttachedActors[0]);
		}

		// If the ball exists.
		if (Ball)
		{
			// Detach it from the dog, enable physics, and set the dog's carrying ball variable to false.
			Ball->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Ball->Mesh->SetSimulatePhysics(true);
			Dog->SetCarryingBall(false);

			// Set the blackboard value of HasBall to false.
			AIController->GetBlackboardComponent()->SetValueAsBool(HasBall.SelectedKeyName, false);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
