#include "BloodlustAwakening/GameMode/CppGamemode.h"

#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

ACppGamemode::ACppGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACppGamemode::InitGame(const FString& InMapName, const FString& InOptions, FString& InErrorMessage)
{
	Super::InitGame(InMapName, InOptions, InErrorMessage);
	OnInitGame();
	UCppTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UCppTeamSubsystem>();
	UCppTeam* BloodRaceTeam = TeamSubsystem->CreateTeam(TEXT("BloodRace"));
	UCppTeam* HumanRaceTeam = TeamSubsystem->CreateTeam(TEXT("HumanRace"));
	UCppTeam* RuinsTeam = TeamSubsystem->CreateTeam(TEXT("Ruins"));
	TeamSubsystem->CreateTeamLink(BloodRaceTeam, HumanRaceTeam, ECppTeamRelationship::Hostile);
	TeamSubsystem->CreateTeamLink(BloodRaceTeam, RuinsTeam, ECppTeamRelationship::Neutral);
	TeamSubsystem->CreateTeamLink(HumanRaceTeam, RuinsTeam, ECppTeamRelationship::Neutral);
}

void ACppGamemode::BeginPlay()
{
	Super::BeginPlay();
}

void ACppGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
