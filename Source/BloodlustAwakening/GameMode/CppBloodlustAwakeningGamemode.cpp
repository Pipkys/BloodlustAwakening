#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGamemode.h"

ACppBloodlustAwakeningGamemode::ACppBloodlustAwakeningGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACppBloodlustAwakeningGamemode::InitGame(const FString& InMapName, const FString& InOptions, FString& InErrorMessage)
{
	Super::InitGame(InMapName, InOptions, InErrorMessage);
	OnInitGame();
}

void ACppBloodlustAwakeningGamemode::BeginPlay()
{
	Super::BeginPlay();
}

void ACppBloodlustAwakeningGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
