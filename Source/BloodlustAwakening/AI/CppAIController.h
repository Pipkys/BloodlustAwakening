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
	AActor* CurrentTarget = nullptr;
	float AcceptableRadius = 0.f;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Controller|AI")
	AActor* GetCurrentTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Controller|AI")
	float GetAcceptableRadius() const;

	UFUNCTION(BlueprintCallable, Category = "Controller|AI")
	void SetCurrentTarget(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Controller|AI")
	void SetAcceptableRadius(float InAcceptableRadius);
};
