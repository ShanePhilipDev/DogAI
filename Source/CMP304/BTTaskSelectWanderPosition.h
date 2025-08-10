// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskSelectWanderPosition.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UBTTaskSelectWanderPosition : public UBTTaskNode
{
	GENERATED_BODY()

	// Constructor
	UBTTaskSelectWanderPosition();

	// Function to execute the task node.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// Key for editing the blackboard.
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WanderPosition;

	// The distance that the dog will wander to.
	UPROPERTY(EditAnywhere)
	float WanderRadius;
};
