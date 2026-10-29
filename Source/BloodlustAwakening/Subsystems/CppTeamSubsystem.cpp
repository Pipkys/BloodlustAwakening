#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

UCppTeam* UCppTeamSubsystem::GetTeam(const FName& InName)
{
	UCppTeam** Found = Teams.FindByPredicate([&InName](UCppTeam* InTeam) { return InTeam->Name == InName; });
	if (!Found)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::GetTeam: Team '%s' not found"), InName.ToString().GetCharArray().GetData());
		return nullptr;
	}
	else
	{
		return *Found;
	}
}

UCppTeamLink* UCppTeamSubsystem::GetTeamLink(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity)
{
	UCppTeamLink** Found = Links.FindByPredicate([InFirstEntity, InSecondEntity](UCppTeamLink* InTeamLink)
	{
		return (InTeamLink->FirstTeam == InFirstEntity->GetTeam()
			&& InTeamLink->SecondTeam == InSecondEntity->GetTeam())
			|| (InTeamLink->FirstTeam == InSecondEntity->GetTeam()
				&& InTeamLink->SecondTeam == InFirstEntity->GetTeam());
	});
	if (!Found)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::GetTeamLink: Team link between '%s' (Team '%s') and '%s' (Team '%s') not found"),
			InFirstEntity.GetObject()->GetName().GetCharArray().GetData(),
			InFirstEntity->GetTeam()->Name.ToString().GetCharArray().GetData(),
			InSecondEntity.GetObject()->GetName().GetCharArray().GetData(),
			InSecondEntity->GetTeam()->Name.ToString().GetCharArray().GetData());
		return nullptr;
	}
	else
	{
		return *Found;
	}
}

bool UCppTeamSubsystem::IsFriendly(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	UCppTeamLink* TeamLink = ((UCppTeamSubsystem*)this)->GetTeamLink(InFirstEntity, InSecondEntity);
	if (!TeamLink)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::IsFriendly: Team link not found"));
		return false;
	}
	return TeamLink->Relationship == ECppTeamRelationship::Friendly;
}

bool UCppTeamSubsystem::IsNeutral(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	UCppTeamLink* TeamLink = ((UCppTeamSubsystem*)this)->GetTeamLink(InFirstEntity, InSecondEntity);
	if (!TeamLink)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::IsNeutral: Team link not found"));
		return false;
	}
	return TeamLink->Relationship == ECppTeamRelationship::Neutral;
}

bool UCppTeamSubsystem::IsFriendlyOrNeutral(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	UCppTeamLink* TeamLink = ((UCppTeamSubsystem*)this)->GetTeamLink(InFirstEntity, InSecondEntity);
	if (!TeamLink)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::IsFriendlyOrNeutral: Team link not found"));
		return false;
	}
	return TeamLink->Relationship == ECppTeamRelationship::Friendly || TeamLink->Relationship == ECppTeamRelationship::Neutral;
}

bool UCppTeamSubsystem::IsHostile(const TScriptInterface<ICppEntity> InFirstEntity, const TScriptInterface<ICppEntity> InSecondEntity) const
{
	UCppTeamLink* TeamLink = ((UCppTeamSubsystem*)this)->GetTeamLink(InFirstEntity, InSecondEntity);
	if (!TeamLink)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::IsHostile: Team link not found"));
		return false;
	}
	return TeamLink->Relationship == ECppTeamRelationship::Hostile;
}

UCppTeam* UCppTeamSubsystem::CreateTeam(const FName& InName)
{
	if (UCppTeam** Found = Teams.FindByPredicate([&InName](UCppTeam* InTeam) { return InTeam->Name == InName; }))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppTeamSubsystem::CreateTeam: Team '%s' already exists, returned this team"), InName.ToString().GetCharArray().GetData());
		return *Found;
	}

	UCppTeam* NewTeam = NewObject<UCppTeam>();
	NewTeam->Name = InName;
	Teams.Add(NewTeam);
	return NewTeam;
}

UCppTeamLink* UCppTeamSubsystem::CreateTeamLink(UCppTeam* InFirstTeam, UCppTeam* InSecondTeam, ECppTeamRelationship InTeamRelationship)
{
	if (InFirstTeam->Name == InSecondTeam->Name)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppTeamSubsystem::CreateTeamLink: Attempting to create team link between the same teams '%s'"), 
			InFirstTeam->Name.ToString().GetCharArray().GetData());
		return nullptr;
	}

	if (UCppTeamLink** Found = Links.FindByPredicate([InFirstTeam, InSecondTeam](UCppTeamLink* InTeamLink)
	{
		return (InTeamLink->FirstTeam == InFirstTeam || InTeamLink->FirstTeam == InSecondTeam)
			&& (InTeamLink->SecondTeam == InFirstTeam || InTeamLink->SecondTeam == InSecondTeam);
	}))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppTeamSubsystem::CreateTeamLink: Team link between teams '%s' and '%s' already exists, returned this team link"), 
			InFirstTeam->Name.ToString().GetCharArray().GetData(),
			InSecondTeam->Name.ToString().GetCharArray().GetData());
		return *Found;
	}

	UCppTeamLink* NewLink = NewObject<UCppTeamLink>();
	NewLink->FirstTeam = InFirstTeam;
	NewLink->SecondTeam = InSecondTeam;
	NewLink->Relationship = InTeamRelationship;
	Links.Add(NewLink);
	return NewLink;
}
