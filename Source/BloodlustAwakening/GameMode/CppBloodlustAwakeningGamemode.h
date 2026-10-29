#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameMode.h"

#include "CppBloodlustAwakeningGamemode.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppBloodlustAwakeningGamemode : public AGameMode
{
	GENERATED_BODY()

public:
	ACppBloodlustAwakeningGamemode();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Initialization")
	void OnInitGame();

	void InitGame(const FString& InMapName, const FString& InOptions, FString& InErrorMessage) override;
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
};
