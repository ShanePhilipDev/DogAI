// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMBaseState.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "FSMWanderState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMWanderState : public UFSMBaseState
{
	GENERATED_BODY()

public:
	// Constructor and destructor.
	UFSMWanderState();
	~UFSMWanderState();

	// Run and switch state functions.
	virtual void Run(float DeltaSeconds) override;
	virtual bool SwitchState(FName& NewState) override;

protected:
	// 
	FVector WanderPosition;
	float WanderRadius;

	// Timer variables
	FTimerHandle IdleTimer;
	float WaitTime;

	// Reset for start of each run.
	bool bFirstRun;

	// Function to determine the new wander position.
	void FindNewPosition();
};
