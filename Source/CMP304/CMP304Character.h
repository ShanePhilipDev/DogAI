// The player class.
// Adapted from Epic Games' first person character class.
// This class gives the user an interface to interact with the dog AI.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerToolbar.h"
#include "Kismet/KismetMathLibrary.h"
#include "DogBall.h"
#include "DogTreat.h"
#include "Engine/World.h"
#include "Dog.h"

#include "CMP304Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class ADog;

UCLASS(config=Game)
class ACMP304Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	
public:
	// Constructor.
	ACMP304Character();

	// The dog ball and treat are meshes attached to the player. When thrown, the meshes are hidden and the relevant actor is spawned.
	// The ball and treat are kept as meshes rather than objects of their own class to prevent them from showing up in searches for actors, which would need to be filtered out so as to not confuse the dog AI.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* DogBall;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* DogTreat;

	// Whether the player currently possesses the ball and treat.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bHasBall;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bHasTreat;

	// The player has a 'toolbar' with what objects they have selected. These are: an empty hand, a ball, and a treat.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<ToolbarSelection> Tool;
	
protected:
	// Called at start of game
	virtual void BeginPlay();

	// The player's HUD widget, displaying the controls, toolbar, dog hunger bar, and the dog's current AI mode.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UUserWidget* PlayerUIWidget;

	// How far the player can grab, and how strong they can throw objects.
	float GrabDistance;
	float ThrowStrength;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	
	// Throws the currently selected object.
	void OnFire();

	// Throw the ball.
	UFUNCTION(BlueprintCallable)
	void ThrowBall();

	// Throw the treat.
	UFUNCTION(BlueprintCallable)
	void ThrowTreat();

	// Functions for selecting tool.
	// ***** //
	void SelectTool1();

	void SelectTool2();

	void SelectTool3();

	void SwapTool(float Val);
	// ***** //

	// Pick up objects.
	void Interact();

	// Tell the dog to switch AI mode.
	void ToggleAIMode();

	// Exit the game.
	void Quit();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

