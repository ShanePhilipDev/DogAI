// The dog's AI controller. Used for running the behavior tree, or giving the dog commands through the finite state machine.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"

#include "DogAIController.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API ADogAIController : public AAIController
{
	GENERATED_BODY()

		
public:
	// Constructor
	ADogAIController();
protected:
	// Runs when the game starts
	virtual void BeginPlay() override;

	// Pointers to the behavior tree and its blackboard.
	UBehaviorTree* BehaviorTree;
	UBlackboardData* BlackboardData;
	
};
