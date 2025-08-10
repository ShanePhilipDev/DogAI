// Bark task node. The dog will bark at the player when hungry.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Engine/EngineTypes.h"

#include "BTTaskBark.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UBTTaskBark : public UBTTaskNode
{
	GENERATED_BODY()

	// Constructor
	UBTTaskBark();

	// Function to execute the task node.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// This node can be interrupted, so also has an AbortTask function implementation.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// This node has a timer, so has a tick function to update it.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Timer variables.
	bool bIsTimerSet;
	float Timer;
	float DelayTime;
};
