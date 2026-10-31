#include "BloodlustAwakening/AI/CppAIController.h"

ACppAIController::ACppAIController()
{
	PrimaryActorTick.bCanEverTick = true;

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

void ACppAIController::SetCurrentTarget(AActor* InActor)
{
	GetBlackboardComponent()->SetValueAsObject("CurrentTarget", InActor);
}
