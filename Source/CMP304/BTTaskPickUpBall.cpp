

#include "BTTaskPickUpBall.h"
#include "Dog.h"
#include "DogAIController.h"
#include "DogBall.h"

// Execute the node.
EBTNodeResult::Type UBTTaskPickUpBall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	// Get the AI controller, if it doesn't exist the task fails.
	ADogAIController* AIController = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// Get the dog.
	ADog* Dog = Cast<ADog>(AIController->GetPawn());

	// If the dog exists...
	if (Dog)
	{
		// Get the ball.
		ADogBall* Ball = Cast<ADogBall>(UGameplayStatics::GetActorOfClass(GetWorld(), ADogBall::StaticClass()));

		// If the ball exists...
		if (Ball)
		{
			// Stop simulating physics, attacht the ball to the dog.
			Ball->Mesh->SetSimulatePhysics(false);
			Ball->AttachToComponent(Dog->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "mouthSocket");

			// Set dog booleans and speed.
			Dog->bIsPickingUpBall = true;
			Dog->bHasFetchedBall = true;
			Dog->SetCarryingBall(true);
			Dog->GetCharacterMovement()->MaxWalkSpeed = Dog->GetTrotSpeed();

			// Set blackboard value for HasBall.
			AIController->GetBlackboardComponent()->SetValueAsBool(HasBall.SelectedKeyName, true);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}
