// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskEat.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UBTTaskEat : public UBTTaskNode
{
	GENERATED_BODY()

	// Constructor.
	UBTTaskEat();

	// Function to execute the task node.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// This node has a timer, so has a tick function to update it.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Timer variables.
	bool bIsTimerSet;
	float Timer;
	float DelayTime;
};
