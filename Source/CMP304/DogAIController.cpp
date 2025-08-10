

#include "DogAIController.h"


ADogAIController::ADogAIController()
{
	// Get the blackboard object.
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/FirstPerson/Assets/DogBlackboard"));
	BlackboardData = BlackboardAsset.Object;

	// Get the behavior tree object.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/FirstPerson/Assets/DogBehaviorTree"));
	BehaviorTree = BehaviorTreeAsset.Object;
	
}

void ADogAIController::BeginPlay()
{
	Super::BeginPlay();

	// Set the blackboard to be used by the behavior tree.
	UBlackboardComponent* BlackboardComp;
	UseBlackboard(BlackboardData, BlackboardComp);

	// Get a pointer to the player character.
	FName PlayerKey = "Player";
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Add the player reference to the blackboard.
	GetBlackboardComponent()->SetValueAsObject(PlayerKey, Player);

	// Run the behavior tree.
	RunBehaviorTree(BehaviorTree);
}
