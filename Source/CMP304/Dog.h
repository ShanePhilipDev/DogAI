// The dog class. 
// Most of the dog's functionality is implemented through the finite state machine and behaviour tree via the dog's AI controller.
// This class contains the variables needed to allow the AI methods to work.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CMP304Character.h"
#include "Kismet/GameplayStatics.h"
#include "DogAIController.h"
#include "Components/ArrowComponent.h"
#include "FiniteStateMachineComponent.h"

#include "Dog.generated.h"

class ACMP304Character;
class UFiniteStateMachineComponent;

UCLASS()
class CMP304_API ADog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADog();

	// Used for animation. When the dog picks up the ball, this is set to true. When the animation finishes, this is set to false.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsPickingUpBall;

	// Used for tracking whether the ball has already fetched the ball, to prevent it from picking up the ball again after returning it.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasFetchedBall;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Materials to give a visual indication of what AI method is being used - gold is for FSM, black is for BT.
	UMaterial* GoldDogMaterial;
	UMaterial* BlackDogMaterial;

	// Booleans to track the dog's actions.
	// ***** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsEating;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsBarking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSitting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCarryingBall;
	// ***** //

	// Floats to hold the dog's movement speeds and track its hunger.
	// ***** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TrotSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SprintSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float HungerMeter;
	// ***** //

	// Pointer to the player character.
	ACMP304Character* PlayerRef;

	// The dog's finite state machine.
	UFiniteStateMachineComponent* FSM;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function to switch between the 2 AI modes.
	void ToggleAIMode();

	// Boolean to track which AI mode is being used.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bUseBehaviorTree;

	// Getter and setter functions
	// ***** //
	UFUNCTION(BlueprintCallable)
	bool GetEating()
	{
		return bIsEating;
	}

	UFUNCTION(BlueprintCallable)
	bool GetBarking()
	{
		return bIsBarking;
	}

	UFUNCTION(BlueprintCallable)
	bool GetSitting()
	{
		return bIsSitting;
	}

	UFUNCTION(BlueprintCallable)
	bool GetCarryingBall()
	{
		return bIsCarryingBall;
	}

	UFUNCTION(BlueprintCallable)
	void SetEating(bool b)
	{
		bIsEating = b;
	}

	UFUNCTION(BlueprintCallable)
	void SetBarking(bool b)
	{
		bIsBarking = b;
	}

	UFUNCTION(BlueprintCallable)
	void SetSitting(bool b)
	{
		bIsSitting = b;
	}

	UFUNCTION(BlueprintCallable)
	void SetCarryingBall(bool b)
	{
		bIsCarryingBall = b;
	}

	void SetHungerMeter(float f)
	{
		HungerMeter = f;
	}

	UFUNCTION(BlueprintCallable)
	float GetWalkSpeed()
	{
		return WalkSpeed;
	}

	UFUNCTION(BlueprintCallable)
	float GetTrotSpeed()
	{
		return TrotSpeed;
	}

	UFUNCTION(BlueprintCallable)
	float GetSprintSpeed()
	{
		return SprintSpeed;
	}

	UFUNCTION(BlueprintCallable)
	float GetHungerMeter()
	{
		return HungerMeter;
	}
	// ***** //
};
