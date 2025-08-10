// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskSit.h"
#include "Dog.h"
#include "DogAIController.h"

UBTTaskSit::UBTTaskSit()
{
	// Set timer starting values.
	bIsTimerSet = false;
	Timer = 0;
	DelayTime = 1;

	// Allow this node to tick.
	bNotifyTick = true;
}

// Execute the node.
EBTNodeResult::Type UBTTaskSit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller, if it doesn't exist the task fails.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If dog exists, set the dog to sit.
	if (Dog)
	{
		Dog->SetSitting(true);
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

// Abort the task. This is called when another task with a higher priority needs to execute.
EBTNodeResult::Type UBTTaskSit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller, if it doesn't exist the task fails.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get the dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If the dog exists, set dog to sitting and start timer. When the timer finishes, finish aborting.
	if (Dog)
	{
		Dog->SetSitting(false);

		if (!bIsTimerSet)
		{
			bIsTimerSet = true;
			Timer = 0;
		}

		if (Timer > DelayTime)
		{
			bIsTimerSet = false;
			Timer = 0;
			FinishLatentAbort(OwnerComp);
			return EBTNodeResult::Aborted;
		}
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTaskSit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Update the timer. If the timer exceeds the time delay needed for the animation, attempt to abort the task again.
	if (Timer > DelayTime)
	{
		AbortTask(OwnerComp, NodeMemory);
	}
	else if (bIsTimerSet)
	{
		Timer += DeltaSeconds;
	}
}


