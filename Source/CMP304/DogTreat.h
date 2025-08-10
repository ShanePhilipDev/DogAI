// Dog treat class. A mesh with physics properties.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "DogTreat.generated.h"

UCLASS()
class CMP304_API ADogTreat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADogTreat();

	// The mesh of the treat.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;

	// A collision sphere for the treat, which is used to make its collision area larger for overlapping with the dog and to make it easier for the player to pick it up.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
