#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

UCppTeam* UCppTeamSubsystem::GetTeam(const FName& InName)
{
	UCppTeam** Found = Teams.FindByPredicate([&InName](UCppTeam* InTeam) { return InTeam->Name == InName; });
	if (!Found)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::GetTeam: Team '%s' not found"), *InName.ToString());
		return nullptr;
	}
	return *Found;
}

UCppTeamLink* UCppTeamSubsystem::GetTeamLink(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity)
{
	ICppEntity* FirstEntity = InFirstEntity.GetInterface();
	ICppEntity* SecondEntity = InSecondEntity.GetInterface();
	if (!FirstEntity || !SecondEntity)
	{
		return nullptr;
	}
	UCppTeam* FirstTeam = FirstEntity->GetTeam();
	UCppTeam* SecondTeam = SecondEntity->GetTeam();
	if (!FirstTeam || !SecondTeam)
	{
		return nullptr;
	}
	if (FirstTeam == SecondTeam || FirstTeam->Name == SecondTeam->Name)
	{
		return nullptr;
	}
	UCppTeamLink** Found = Links.FindByPredicate([FirstTeam, SecondTeam](UCppTeamLink* InTeamLink)
	{
		return (InTeamLink->FirstTeam == FirstTeam && InTeamLink->SecondTeam == SecondTeam)
			|| (InTeamLink->FirstTeam == SecondTeam && InTeamLink->SecondTeam == FirstTeam);
	});
	return Found ? *Found : nullptr;
}

bool UCppTeamSubsystem::ResolveRelationship(TScriptInterface<ICppEntity> InFirstEntity,
	TScriptInterface<ICppEntity> InSecondEntity,
	ECppTeamRelationship& OutRelationship) const
{
	ICppEntity* FirstEntity = InFirstEntity.GetInterface();
	ICppEntity* SecondEntity = InSecondEntity.GetInterface();
	if (!FirstEntity || !SecondEntity)
	{
		OutRelationship = ECppTeamRelationship::Neutral;
		return false;
	}
	UCppTeam* FirstTeam = FirstEntity->GetTeam();
	UCppTeam* SecondTeam = SecondEntity->GetTeam();
	if (!FirstTeam || !SecondTeam)
	{
		OutRelationship = ECppTeamRelationship::Neutral;
		return false;
	}
	if (FirstTeam == SecondTeam || FirstTeam->Name == SecondTeam->Name)
	{
		OutRelationship = ECppTeamRelationship::Friendly;
		return true;
	}
	const UCppTeamLink* const* Found = Links.FindByPredicate([FirstTeam, SecondTeam](UCppTeamLink* InTeamLink)
	{
		return (InTeamLink->FirstTeam == FirstTeam && InTeamLink->SecondTeam == SecondTeam)
			|| (InTeamLink->FirstTeam == SecondTeam && InTeamLink->SecondTeam == FirstTeam);
	});
	if (!Found)
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("UCppTeamSubsystem::ResolveRelationship: no team link between '%s' and '%s', treating as neutral"),
			*FirstTeam->Name.ToString(), *SecondTeam->Name.ToString());
		OutRelationship = ECppTeamRelationship::Neutral;
		return false;
	}
	OutRelationship = (*Found)->Relationship;
	return true;
}

bool UCppTeamSubsystem::IsFriendly(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	ECppTeamRelationship Relationship;
	const bool bResolved = ResolveRelationship(InFirstEntity, InSecondEntity, Relationship);
	return bResolved && Relationship == ECppTeamRelationship::Friendly;
}

bool UCppTeamSubsystem::IsNeutral(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	ECppTeamRelationship Relationship;
	ResolveRelationship(InFirstEntity, InSecondEntity, Relationship);
	return Relationship == ECppTeamRelationship::Neutral;
}

bool UCppTeamSubsystem::IsFriendlyOrNeutral(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	ECppTeamRelationship Relationship;
	ResolveRelationship(InFirstEntity, InSecondEntity, Relationship);
	return Relationship == ECppTeamRelationship::Friendly || Relationship == ECppTeamRelationship::Neutral;
}

bool UCppTeamSubsystem::IsHostile(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity) const
{
	ECppTeamRelationship Relationship;
	const bool bResolved = ResolveRelationship(InFirstEntity, InSecondEntity, Relationship);
	return bResolved && Relationship == ECppTeamRelationship::Hostile;
}

UCppTeam* UCppTeamSubsystem::CreateTeam(const FName& InName)
{
	if (UCppTeam** Found = Teams.FindByPredicate([&InName](UCppTeam* InTeam) { return InTeam->Name == InName; }))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppTeamSubsystem::CreateTeam: Team '%s' already exists, returned this team"), *InName.ToString());
		return *Found;
	}
	UCppTeam* NewTeam = NewObject<UCppTeam>();
	NewTeam->Name = InName;
	Teams.Add(NewTeam);
	return NewTeam;
}

UCppTeamLink* UCppTeamSubsystem::CreateTeamLink(UCppTeam* InFirstTeam, UCppTeam* InSecondTeam, ECppTeamRelationship InTeamRelationship)
{
	if (!InFirstTeam || !InSecondTeam)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppTeamSubsystem::CreateTeamLink: invalid team"));
		return nullptr;
	}
	if (InFirstTeam->Name == InSecondTeam->Name)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppTeamSubsystem::CreateTeamLink: Attempting to create team link between the same teams '%s'"),
			*InFirstTeam->Name.ToString());
		return nullptr;
	}
	if (UCppTeamLink** Found = Links.FindByPredicate([InFirstTeam, InSecondTeam](UCppTeamLink* InTeamLink)
	{
		return (InTeamLink->FirstTeam == InFirstTeam || InTeamLink->FirstTeam == InSecondTeam)
			&& (InTeamLink->SecondTeam == InFirstTeam || InTeamLink->SecondTeam == InSecondTeam);
	}))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppTeamSubsystem::CreateTeamLink: Team link between teams '%s' and '%s' already exists, returned this team link"),
			*InFirstTeam->Name.ToString(),
			*InSecondTeam->Name.ToString());
		return *Found;
	}
	UCppTeamLink* NewLink = NewObject<UCppTeamLink>();
	NewLink->FirstTeam = InFirstTeam;
	NewLink->SecondTeam = InSecondTeam;
	NewLink->Relationship = InTeamRelationship;
	Links.Add(NewLink);
	return NewLink;
}
