// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskEat.h"
#include "Dog.h"
#include "DogAIController.h"
#include "DogTreat.h"

UBTTaskEat::UBTTaskEat()
{
	// Set timer starting values.
	bIsTimerSet = false;
	Timer = 0;
	DelayTime = 2.5;

	// Allow this node to tick.
	bNotifyTick = true;
}

// Execute the node.
EBTNodeResult::Type UBTTaskEat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller. If the AI controller doesn't exist, the task fails.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If dog exists...
	if (Dog)
	{
		// Start eating.
		Dog->SetEating(true);

		// Get the treat.
		ADogTreat* Treat = Cast<ADogTreat>(UGameplayStatics::GetActorOfClass(GetWorld(), ADogTreat::StaticClass()));

		// Start the timer and destroy the treat.
		if (Treat)
		{
			if (!bIsTimerSet)
			{
				bIsTimerSet = true;
				Timer = 0;
				Treat->Destroy();
			}
		}

		// When the timer finishes, reset it, reset the dog's hunger and finish the task.
		if (Timer > DelayTime)
		{
			bIsTimerSet = false;
			Timer = 0;
			Dog->SetHungerMeter(1);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTaskEat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Update the timer. If the timer exceeds the time delay needed for the animation, attempt to execute the task again.
	if (Timer > DelayTime)
	{
		ExecuteTask(OwnerComp, NodeMemory);
	}
	else if (bIsTimerSet)
	{
		Timer += DeltaSeconds;
	}
}