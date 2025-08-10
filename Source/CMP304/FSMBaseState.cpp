
#include "FSMBaseState.h"

// State names should be set in the constructor.
UFSMBaseState::UFSMBaseState()
{
	StateName = "Base";
}

UFSMBaseState::~UFSMBaseState()
{
}

// Run the state.
void UFSMBaseState::Run(float DeltaSeconds)
{
}

// Check the conditions needed for switching state. If conditions are met, return true to switch to the state defined by FName NewState.
bool UFSMBaseState::SwitchState(FName& NewState)
{
	return false;
}

// Initialise pointers.
void UFSMBaseState::InitBase(ACharacter* DogRef, ACharacter* PlayerRef, ADogAIController* AIRef, UWorld* WorldRef)
{
	Dog = DogRef;
	Player = PlayerRef;
	AIController = AIRef;
	World = WorldRef;
}