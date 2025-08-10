// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskSit.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UBTTaskSit : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Constructor
	UBTTaskSit();

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
