// Behavior tree service node to update the blackboard data.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceUpdateBlackboard.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UBTServiceUpdateBlackboard : public UBTService
{
	GENERATED_BODY()
		// Constructor
		UBTServiceUpdateBlackboard();

		// Run the node
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// The blackboard keys that will be set in this node.
	// ***** //
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector ShouldChaseBall;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector CanEatTreat;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector PlayerIsHoldingBall;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector PlayerIsHoldingTreat;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector IsHungry;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector Ball;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector Treat;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector ShouldFollowPlayer;
	// ***** //

	// Variables for use within the node.
	bool bPlayerHasBall;
	bool bPlayerHasTreat;
	bool bFollowPlayer;

};
