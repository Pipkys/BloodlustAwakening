#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CppAIController.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACppAIController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|AI")
	TObjectPtr<UBlackboardData> BlackboardData;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Controller|AI")
	void SetCurrentTarget(AActor* InActor);
};
