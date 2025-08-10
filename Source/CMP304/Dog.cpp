// Fill out your copyright notice in the Description page of Project Settings.


#include "Dog.h"

// Sets default values
ADog::ADog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting default values.
	bUseControllerRotationYaw = false;
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 34.0f);

	// Sets the AI controller for the dog.
	AIControllerClass = ADogAIController::StaticClass();

	// Get dog's mesh object.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Dog_Labrador/Meshes/Mesh_Labrador"));
	USkeletalMesh* SkeletalMesh = MeshAsset.Object;

	// Set up mesh.
	GetMesh()->SetSkeletalMesh(SkeletalMesh);	
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Set up animation blueprint.
	FString AnimClassString = "Class'/Game/FirstPerson/Assets/Dog_AnimBP.Dog_AnimBP_C'";
	UClass* AnimationClass = LoadObject<UClass>(NULL, *AnimClassString);
	if (AnimationClass)
	{
		GetMesh()->SetAnimInstanceClass(AnimationClass);
	}

	// Set up dog coat materials.
	static ConstructorHelpers::FObjectFinder<UMaterial> GoldMaterialAsset(TEXT("/Game/Dog_Labrador/Materials/Labrador_color_1"));
	GoldDogMaterial = GoldMaterialAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> BlackMaterialAsset(TEXT("/Game/Dog_Labrador/Materials/Labrador_color_2"));
	BlackDogMaterial = BlackMaterialAsset.Object;

	GetMesh()->SetMaterial(0, BlackDogMaterial);

	// Set up character movement component variables.
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 200;

	// Create the finite state machine.
	FSM = CreateDefaultSubobject<UFiniteStateMachineComponent>(TEXT("FSM"));

	// Set default speed and hunger values.
	HungerMeter = 1;
	WalkSpeed = 200;
	TrotSpeed = 600;
	SprintSpeed = 1000;

	// The dog will use the behavior tree by default.
	bUseBehaviorTree = true;
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = Cast<ACMP304Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FSM->Init(this, PlayerRef, Cast<ADogAIController>(GetController()), GetWorld());
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the player has the ball, reset the dog's fetch status.
	if (PlayerRef->bHasBall)
	{
		bHasFetchedBall = false;
	}

	// Disable dog's movement if the are eating, barking, sitting, or picking up the ball.
	if (bIsEating || bIsBarking || bIsSitting || bIsPickingUpBall)
	{
		GetCharacterMovement()->DisableMovement();
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	}

	// The rate at which the dog's hunger levels deplete.
	float HungerDepletionRate;

	// When the dog is sprinting, their hunger will deplete faster.
	if (GetCharacterMovement()->GetMaxSpeed() == SprintSpeed)
	{
		HungerDepletionRate = 0.05f;
	}
	else
	{
		HungerDepletionRate = 0.02f;
	}

	// Set hunger meter.
	HungerMeter -= HungerDepletionRate * DeltaTime;

	// If the dog isn't using the behavior tree, run the finite state machine.
	if (!bUseBehaviorTree)
	{
		if (FSM)
		{
			FSM->Update(DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Switches AI mode and sets new mesh.
void ADog::ToggleAIMode()
{
	// Invert current AI selection.
	bUseBehaviorTree = !bUseBehaviorTree;

	// Get AI controller.
	ADogAIController* AIController = Cast<ADogAIController>(GetController());

	if (AIController)
	{
		// If using the behavior tree, the dog will have a black coat, the focus will be cleared and the AI controller's brain component will run (this runs the behavior tree).
		if (bUseBehaviorTree)
		{
			GetMesh()->SetMaterial(0, BlackDogMaterial);
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
			AIController->GetBrainComponent()->StartLogic();
		}
		else // Otherwise use the finite state machine. The brain logic will be stopped, and the finite state machine will give commands to the AI controller instead of the behavior tree.
		{
			GetMesh()->SetMaterial(0, GoldDogMaterial);
			AIController->GetBrainComponent()->StopLogic("Switch to FSM");
		}
	}
}

