// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMBaseState.h"
#include "DogBall.h"
#include "FSMReturningBallState.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UFSMReturningBallState : public UFSMBaseState
{
	GENERATED_BODY()

public:
	// Constructor and destructor.
	UFSMReturningBallState();
	~UFSMReturningBallState();

	// Run and switch state functions.
	virtual void Run(float DeltaSeconds) override;
	virtual bool SwitchState(FName& NewState) override;

private:
	// Pointer to the dog ball.
	ADogBall* DogBall;

	// Timer variables.
	FTimerHandle WaitTimer;
	float WaitTime;

	// A function to drop the ball.
	void DropBall();
};
