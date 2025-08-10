// Dog ball class. A mesh with bouncy physics properties.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "DogBall.generated.h"

UCLASS()
class CMP304_API ADogBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADogBall();

	// The mesh of the ball.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	// A collision sphere for the ball, which is used to make its collision area larger for overlapping with the dog and to make it easier for the player to pick it up.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* CollisionSphere;

	// Special physics properties to make the ball bouncy.
	UPhysicalMaterial* BallPhysics;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
