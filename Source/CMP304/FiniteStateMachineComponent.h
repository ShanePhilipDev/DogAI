// The finite state machine class.
// The state machine has 6 different states.
// The machine's update function runs the currently selected state.
// When running, it first checks if the conditions are met for a state change. Then it runs the state itself.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DogAIController.h"
#include "FSMBaseState.h"
#include "FSMWanderState.h"
#include "FSMFetchingBallState.h"
#include "FSMReturningBallState.h"
#include "FSMSittingState.h"
#include "FSMEatingState.h"
#include "FSMBarkingState.h"
#include "Engine/World.h"

#include "FiniteStateMachineComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CMP304_API UFiniteStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFiniteStateMachineComponent();

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The current state of the state machine.
	UPROPERTY()
	UFSMBaseState* CurrentState;

	// States are stored in a map, with the state's name used as a key.
	UPROPERTY()
	TMap<FName, UFSMBaseState*> States;

	// Each of the state machine's states.
	// States are setup as UPROPERTYs to prevent the pointers from being destroyed in Unreal's garbage collection.
	// ***** //
	UPROPERTY()
	UFSMWanderState* WanderState;

	UPROPERTY()
	UFSMFetchingBallState* FetchingBallState;

	UPROPERTY()
	UFSMReturningBallState* ReturningBallState;

	UPROPERTY()
	UFSMSittingState* SittingState;

	UPROPERTY()
	UFSMEatingState* EatingState;

	UPROPERTY()
	UFSMBarkingState* BarkingState;
	// ***** //

	// Pointers to objects that are needed by the states.
	ACharacter* Dog;
	ACharacter* Player;
	ADogAIController* AIController;

public:	
	// Initialise the state machine and all of its states.
	void Init(ACharacter* DogRef, ACharacter* PlayerRef, ADogAIController* AIRef, UWorld* WorldRef);

	// Run the state machine.
	void Update(float DeltaSeconds);

};
