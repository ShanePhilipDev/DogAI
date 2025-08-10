// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMBaseState.h"
#include "FSMBarkingState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMBarkingState : public UFSMBaseState
{
	GENERATED_BODY()
	
public:
	// Constructor
	UFSMBarkingState();

	// Run and switch state functions.
	virtual void Run(float DeltaSeconds) override;
	virtual bool SwitchState(FName& NewState) override;

protected:
	// Timer variables.
	FTimerHandle StopBarkingTimer;
	float StopBarkingTime;

	// Bool for whether the dog has finished barking.
	bool bFinishedBarking;

	// A function to stop the barking.
	void StopBarking();
};
