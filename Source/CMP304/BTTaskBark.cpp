

#include "BTTaskBark.h"
#include "Dog.h"
#include "DogAIController.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTaskBark::UBTTaskBark()
{
	// Set timer starting values.
	bIsTimerSet = false;
	Timer = 0;
	DelayTime = 0.8;

	// Allow this node to tick.
	bNotifyTick = true;
}

// Execute the node.
EBTNodeResult::Type UBTTaskBark::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	// If AI controller doesn't exist, the node fails.
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get the dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If the dog exists, start barking. The node will be marked as in progress.
	if (Dog)
	{
		Dog->SetBarking(true);
		return EBTNodeResult::InProgress;
	}
	else
	{
		// Fails if dog doesn't exist.
		return EBTNodeResult::Failed;
	}
}

// Abort the task. This is called when another task with a higher priority needs to execute.
EBTNodeResult::Type UBTTaskBark::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	// If AI controller doesn't exist, the node fails.
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get the dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If the dog exists, set barking to false. The animation will still need to finish, so a timer is started to delay finishing the abort. Once the timer has finished, the abort will finish and the tree can move onto the next task.
	if (Dog)
	{
		Dog->SetBarking(false);

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

void UBTTaskBark::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Update the timer. If the timer exceeds the time delay needed for the animation, attempt to abort the task again.
	if (Timer > DelayTime)
	{
		AbortTask(OwnerComp, NodeMemory);
	}
	else if(bIsTimerSet)
	{
		Timer += DeltaSeconds;
	}
}
