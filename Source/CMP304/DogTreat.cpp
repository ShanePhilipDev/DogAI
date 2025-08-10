// Fill out your copyright notice in the Description page of Project Settings.


#include "DogTreat.h"

// Sets default values
ADogTreat::ADogTreat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get the treat's mesh.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TreatAsset(TEXT("/Game/FirstPerson/Assets/DogTreatMesh"));
	UStaticMesh* TreatMesh = TreatAsset.Object;

	// The root component will be the treat's mesh.
	SetRootComponent(Mesh);

	// Set up the mesh.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(TreatMesh);
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
void ADogTreat::BeginPlay()
{
	Super::BeginPlay();

	// Simulate physics for the treat.
	Mesh->SetSimulatePhysics(true);
}

// Called every frame
void ADogTreat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

