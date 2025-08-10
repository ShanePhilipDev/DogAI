// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP304Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId



DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACMP304Character

ACMP304Character::ACMP304Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Get skeletal mesh asset.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms"));
	USkeletalMesh* SkeletalMesh = MeshAsset.Object;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetSkeletalMesh(SkeletalMesh);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(0.f, 20.f, 0.f));
	Mesh1P->SetRelativeLocation(FVector(-3.9f, 5.f, -155.7f));
	Mesh1P->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f));

	// Set up animation blueprint.
	FString AnimClassString = "Class'/Game/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'";
	UClass* AnimationClass = LoadObject<UClass>(NULL, *AnimClassString);
	if (AnimationClass)
	{
		Mesh1P->SetAnimInstanceClass(AnimationClass);
	}
	
	// Set up HUD.
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FirstPerson/Assets/PlayerUI"));
	PlayerUIWidget = CreateWidget(GetWorld(), WidgetClass.Class);

	// Set up dog ball mesh.
	// ***** //
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallAsset(TEXT("/Engine/BasicShapes/Sphere"));
	UStaticMesh* BallMesh = BallAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> BallMaterialAsset(TEXT("/Game/FirstPerson/Assets/BallMaterial"));
	UMaterial* BallMaterial = BallMaterialAsset.Object;

	DogBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DogBallMesh"));
	DogBall->SetStaticMesh(BallMesh);
	DogBall->SetMaterial(0, BallMaterial);
	DogBall->SetRelativeScale3D(FVector(0.08f, 0.08f, 0.08f));
	DogBall->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// ***** //

	// Set up dog treat mesh.
	// ***** //
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TreatAsset(TEXT("/Game/FirstPerson/Assets/DogTreatMesh"));
	UStaticMesh* TreatMesh = TreatAsset.Object;

	DogTreat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DogTreatMesh"));
	DogTreat->SetStaticMesh(TreatMesh);
	DogTreat->SetRelativeScale3D(FVector(0.06f, 0.06f, 0.06f));
	DogTreat->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// ***** //

	// Set default values.
	bHasBall = true;
	bHasTreat = true;
	GrabDistance = 400;
	ThrowStrength = 100;
}

void ACMP304Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Attach meshes to the hand.
	DogBall->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_Socket"));
	DogTreat->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_Socket"));
	DogTreat->SetRelativeRotation(FRotator(135.0f, -30.0f, 0.0f));

	// Add HUD to viewport.
	if (PlayerUIWidget)
	{
		PlayerUIWidget->AddToViewport();
	}
}

// Called every frame
void ACMP304Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Sets the visibility of the player's ball and treat based on their tool selection.
	// ***** //
	switch (Tool)
	{
	case ToolbarSelection::Nothing:
		DogBall->SetVisibility(false);
		DogTreat->SetVisibility(false);
		break;
	case ToolbarSelection::Ball:
		DogTreat->SetVisibility(false);
		if (bHasBall)
		{
			DogBall->SetVisibility(true);
		}
		break;
	case ToolbarSelection::Treat:
		if (!UGameplayStatics::GetActorOfClass(GetWorld(), ADogTreat::StaticClass()))
		{
			bHasTreat = true;
		}

		DogBall->SetVisibility(false);
		if (bHasTreat)
		{
			DogTreat->SetVisibility(true);
		}
		break;
	default:
		break;
	}
	// ***** //
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACMP304Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACMP304Character::OnFire);

	// Bind toolbar select inputs
	PlayerInputComponent->BindAction("Toolbar1", IE_Pressed, this, &ACMP304Character::SelectTool1);
	PlayerInputComponent->BindAction("Toolbar2", IE_Pressed, this, &ACMP304Character::SelectTool2);
	PlayerInputComponent->BindAction("Toolbar3", IE_Pressed, this, &ACMP304Character::SelectTool3);
	PlayerInputComponent->BindAxis("SwapTool", this, &ACMP304Character::SwapTool);

	// Bind interact input
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACMP304Character::Interact);

	// Bind AI toggle input
	PlayerInputComponent->BindAction("ToggleAIMode", IE_Pressed, this, &ACMP304Character::ToggleAIMode);

	// Bind quit input
	PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &ACMP304Character::Quit);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACMP304Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACMP304Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACMP304Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACMP304Character::LookUpAtRate);
}

void ACMP304Character::OnFire()
{
	// Switch statement to choose the action based on toolbar selection.
	// Only calls the relevant throw function if they actually possess the object.
	// ***** //
	switch (Tool)
	{
	case ToolbarSelection::Nothing:
		break;
	case ToolbarSelection::Ball:
		if (bHasBall)
		{
			ThrowBall();
		}
		break;
	case ToolbarSelection::Treat:
		if (bHasTreat)
		{
			ThrowTreat();
		}
		break;
	default:
		break;
	}
	// ***** //
}

void ACMP304Character::ThrowBall()
{
	// Spawns a ball at the position of the ball in the player's hand.
	ADogBall* SpawnedBall;
	SpawnedBall = GetWorld()->SpawnActor<ADogBall>(ADogBall::StaticClass(), DogBall->GetComponentTransform());

	// Launches the ball forwards.
	FVector Impulse = FirstPersonCameraComponent->GetForwardVector() * ThrowStrength;
	SpawnedBall->Mesh->AddImpulse(Impulse);

	// Hide the ball mesh attached to the player, and set player to no longer posssess the ball.
	DogBall->SetVisibility(false);
	bHasBall = false;
}

void ACMP304Character::ThrowTreat()
{
	// Spawns a treat at the position of the treat in the player's hand.
	ADogTreat* SpawnedTreat;
	SpawnedTreat = GetWorld()->SpawnActor<ADogTreat>(ADogTreat::StaticClass(), DogTreat->GetComponentTransform());

	// Launches the treat forwards.
	FVector Impulse = FirstPersonCameraComponent->GetForwardVector() * ThrowStrength;
	SpawnedTreat->Mesh->AddImpulse(Impulse);

	// Hide the treat mesh attached to the player, and set player to no longer posssess the treat.
	DogTreat->SetVisibility(false);
	bHasTreat = false;
}

// Set tool selection to nothing.
void ACMP304Character::SelectTool1()
{
	Tool = ToolbarSelection::Nothing;
}

// Set tool selection to ball.
void ACMP304Character::SelectTool2()
{
	Tool = ToolbarSelection::Ball;
}

// Set tool selection to treat.
void ACMP304Character::SelectTool3()
{
	Tool = ToolbarSelection::Treat;
}

// Attempt to pick up an object.
void ACMP304Character::Interact()
{
	// Prepare line trace hit result.
	FHitResult Hit(ForceInit);

	// Define line trace start and end points.
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * GrabDistance);

	// Setup collision parameters.
	FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
	//CollisionParams.TraceTag = GetWorld()->DebugDrawTraceTag;

	// If line trace hits an object...
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		// If it hits a ball, set the player to possess the ball and destroy the ball actor in the world.
		if (Cast<ADogBall>(Hit.GetActor()) != NULL)
		{
			bHasBall = true;
			Hit.GetActor()->Destroy();
		}
		else if (Cast<ADogTreat>(Hit.GetActor()) != NULL) // Otherwise, if it hits the treat, set the player to possess the treat and destroy the treat actor in the world.
		{
			bHasTreat = true;
			Hit.GetActor()->Destroy();
		}
	}
}

void ACMP304Character::ToggleAIMode()
{
	// Get the dog.
	ADog* Dog = Cast<ADog>(UGameplayStatics::GetActorOfClass(GetWorld(), ADog::StaticClass()));

	// If the dog exists, change its AI mode.
	if (Dog)
	{
		Dog->ToggleAIMode();
	}
}

// Swaps the currently selected tool based on mouse wheel input.
void ACMP304Character::SwapTool(float Val)
{
	UINT8 Input = Val;
	UINT8 Selection = (UINT8)Tool;
	
	Selection += Input;

	switch (Selection)
	{
	case 0:
		Tool = ToolbarSelection::Nothing;
		break;
	case 1:
		Tool = ToolbarSelection::Ball;
		break;
	case 2:
		Tool = ToolbarSelection::Treat;
		break;
	}
	
}

// Exits the game.
void ACMP304Character::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, true);
}


void ACMP304Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACMP304Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACMP304Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACMP304Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


