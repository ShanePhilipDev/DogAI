// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMBaseState.h"
#include "FSMSittingState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMSittingState : public UFSMBaseState
{
	GENERATED_BODY()
	
public:
	// Constructor
	UFSMSittingState();

	// Run and switch state functions.
	virtual void Run(float DeltaSeconds) override;
	virtual bool SwitchState(FName& NewState) override;

protected:
	// Timer variables
	FTimerHandle StandUpTimer;
	float StandUpTime;
	
	// Boolean for whether the dog is standing.
	bool bIsStanding;

	// Function to make the dog stand up.
	void StandUp();
};
