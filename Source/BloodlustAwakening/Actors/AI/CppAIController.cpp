#include "BloodlustAwakening/Actors/AI/CppAIController.h"

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

	RunBehaviorTree(BehaviorTreeAsset);
	Blackboard->InitializeBlackboard(*BlackboardData);
}

void ACppAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppAIController::SetCurrentTarget(AActor* InActor)
{
	GetBlackboardComponent()->SetValueAsObject("CurrentTarget", InActor);
}
