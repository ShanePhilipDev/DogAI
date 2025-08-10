// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMBaseState.h"
#include "DogBall.h"
#include "FSMFetchingBallState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMFetchingBallState : public UFSMBaseState
{
	GENERATED_BODY()
	
public:
	// Constructor and destructor.
	UFSMFetchingBallState();
	~UFSMFetchingBallState();

	// Run and switch state functions.
	virtual void Run(float DeltaSeconds) override;
	virtual bool SwitchState(FName& NewState) override;

private:
	// Pointer to the dog ball.
	ADogBall* DogBall;
};
