// Base state class.
// All other states inherit from this class. Provides the main variables and functions (to be overidden) that each state needs.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DogAIController.h"
#include "Engine/World.h"
#include "FSMBaseState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMBaseState : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and destructor.
	UFSMBaseState();
	~UFSMBaseState();

	// Virtual functions for running the state, and for checking if it needs to switch state.
	// Each unique state should override these functions.
	virtual void Run(float DeltaSeconds);
	virtual bool SwitchState(FName& NewState);

	// Initialise the state's pointers.
	void InitBase(ACharacter* DogRef, ACharacter* PlayerRef, ADogAIController* AIRef, UWorld* WorldRef);

	// Returns the name of the state.
	FName GetName()
	{
		return StateName;
	}

protected:
	// The name of the state. Used as a key in the map of states in the state machine.
	FName StateName;

	// Dog and player are stored as characters rather than their own class to avoid some spooky circular dependency issues.
	// Can cast dog and player to their relevant classes in the state's functions if needed.
	ACharacter* Dog;
	ACharacter* Player;

	// Pointer to the AI controller.
	ADogAIController* AIController;

	// Need pointer to the world as UObjects don't have access to the world.
	UWorld* World; 
};
