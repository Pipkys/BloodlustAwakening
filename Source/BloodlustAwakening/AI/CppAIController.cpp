#include "BloodlustAwakening/AI/CppAIController.h"

ACppAIController::ACppAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	BrainComponent = BehaviorTreeComponent;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
}

void ACppAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardData)
	{
		Blackboard->InitializeBlackboard(*BlackboardData);
	}

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void ACppAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* ACppAIController::GetCurrentTarget() const
{
	return CurrentTarget;
}

float ACppAIController::GetAcceptableRadius() const
{
	return AcceptableRadius;
}

void ACppAIController::SetCurrentTarget(AActor* InActor)
{
	CurrentTarget = InActor;
	Blackboard->SetValueAsObject("CurrentTarget", InActor);
}

void ACppAIController::SetAcceptableRadius(float InAcceptableRadius)
{
	AcceptableRadius = InAcceptableRadius;
	Blackboard->SetValueAsFloat("AcceptableRadius", InAcceptableRadius);
}
