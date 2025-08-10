// Fill out your copyright notice in the Description page of Project Settings.


#include "DogBall.h"

// Sets default values
ADogBall::ADogBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get mesh.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallAsset(TEXT("/Engine/BasicShapes/Sphere"));
	UStaticMesh* BallMesh = BallAsset.Object;

	// Get material.
	static ConstructorHelpers::FObjectFinder<UMaterial> BallMaterialAsset(TEXT("/Game/FirstPerson/Assets/BallMaterial"));
	UMaterial* BallMaterial = BallMaterialAsset.Object;

	// Get the ball physics properties.
	static ConstructorHelpers::FObjectFinder<UObject> BallPhysicsAsset(TEXT("/Game/FirstPerson/Assets/BallPhysics"));
	BallPhysics = (UPhysicalMaterial*)BallPhysicsAsset.Object;

	// Set the mesh as the root component.
	SetRootComponent(Mesh);

	// Set up the mesh.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(BallMesh);
	Mesh->SetMaterial(0, BallMaterial);
	Mesh->BodyInstance.bOverrideMass = true;
	Mesh->BodyInstance.SetMassOverride(0.1);
	Mesh->SetCollisionProfileName(TEXT("PhysicsBody"));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECR_Ignore);
	
	// Set up the collision sphere.
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(Mesh);
	CollisionSphere->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	CollisionSphere->SetCollisionProfileName(TEXT("PhysicsBody"));
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetVisibility(false);
}

// Called when the game starts or when spawned
void ADogBall::BeginPlay()
{
	Super::BeginPlay();

	// Enable the ball's physics.
	Mesh->SetPhysMaterialOverride(BallPhysics);
	Mesh->SetSimulatePhysics(true);
}

// Called every frame
void ADogBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

