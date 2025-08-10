// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMBaseState.h"
#include "DogTreat.h"
#include "FSMEatingState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMEatingState : public UFSMBaseState
{
	GENERATED_BODY()
	
public:
	// Constructor
	UFSMEatingState();

	// Run and switch state functions.
	virtual void Run(float DeltaSeconds) override;
	virtual bool SwitchState(FName& NewState) override;

protected:
	// Pointer to the dog treat.
	ADogTreat* DogTreat;

	// Timer variables.
	FTimerHandle EatTimer;
	float EatTime;

	// Bool for storing whether the dog has finished eating.
	bool bFinishedEating;
	
	// Function to finish eating.
	void FinishEating();
};
