
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskPickUpBall.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UBTTaskPickUpBall : public UBTTaskNode
{
	GENERATED_BODY()

	// Function to execute the task node.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// Key for editing the blackboard.
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HasBall;
};
