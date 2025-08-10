// Fill out your copyright notice in the Description page of Project Settings.


#include "FiniteStateMachineComponent.h"

// Sets default values for this component's properties
UFiniteStateMachineComponent::UFiniteStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
	// ...
}


// Called when the game starts
void UFiniteStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Set up states and references.
void UFiniteStateMachineComponent::Init(ACharacter* DogRef, ACharacter* PlayerRef, ADogAIController* AIRef, UWorld* WorldRef)
{
	// Pointers to needed objects.
	Dog = DogRef;
	Player = PlayerRef;
	AIController = AIRef;

	// Setting up the states and adding them to the map of states.
	// ***** //
	WanderState = NewObject<UFSMWanderState>();
	WanderState->InitBase(Dog, Player, AIController, WorldRef);
	States.Add(WanderState->GetName(), WanderState);

	FetchingBallState = NewObject<UFSMFetchingBallState>();
	FetchingBallState->InitBase(Dog, Player, AIController, WorldRef);
	States.Add(FetchingBallState->GetName(), FetchingBallState);

	ReturningBallState = NewObject<UFSMReturningBallState>();
	ReturningBallState->InitBase(Dog, Player, AIController, WorldRef);
	States.Add(ReturningBallState->GetName(), ReturningBallState);

	SittingState = NewObject<UFSMSittingState>();
	SittingState->InitBase(Dog, Player, AIController, WorldRef);
	States.Add(SittingState->GetName(), SittingState);

	EatingState = NewObject<UFSMEatingState>();
	EatingState->InitBase(Dog, Player, AIController, WorldRef);
	States.Add(EatingState->GetName(), EatingState);

	BarkingState = NewObject<UFSMBarkingState>();
	BarkingState->InitBase(Dog, Player, AIController, WorldRef);
	States.Add(BarkingState->GetName(), BarkingState);
	// ***** //

	// The starting state is the wander state.
	CurrentState = WanderState;
}

void UFiniteStateMachineComponent::Update(float DeltaSeconds)
{
	// New state name. Passed into the current state's switch state function. If the state needs to be changed, this key will contain the new state's name.
	FName NewState = " ";

	// If the current state is valid...
	if (CurrentState)
	{
		// Check the state switch conditions for the current state.
		if (CurrentState->SwitchState(NewState))
		{
			// Switch to the new state if conditions are met.
			CurrentState = *States.Find(NewState);

			// If new state is not valid, revert back to the wander state.
			if (CurrentState == nullptr)
			{
				CurrentState = WanderState;
			}
		}
		
		// Run the current state.
		CurrentState->Run(DeltaSeconds);
	}
}

