// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "KwangGameplayTags.h"
#include "AbilitySystem/Abilities/KwangGameplayAbility.h"

void UDynastyAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag &InputTag)
{

    if(!InputTag.IsValid())
    {
        return;
    }
    for(FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		if (InputTag.MatchesTag(KwangGameplayTags::InputTag_Toggleable))
		{
			// 토글 가능 태그 일 경우 누를 때마다 온 오프가 되어야 한다.
			if (AbilitySpec.IsActive())
			{
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		else
		{
			if (!TryActivateAbility(AbilitySpec.Handle))
			{
				AbilitySpecInputPressed(AbilitySpec);
			}
		}
		
    }
}

void UDynastyAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag &InputTag)
{
	if (!InputTag.IsValid() || !InputTag.MatchesTag(KwangGameplayTags::InputTag_MustBeHeld))
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}

	}
}

void UDynastyAbilitySystemComponent::GrantPlayerWeaponAbility(const TArray<FKwangAbilitySet>& InDefaultWeaponAbilities,int32 ApplyLevel)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FKwangAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if(!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		GiveAbility(AbilitySpec);
	}

}

void UDynastyAbilitySystemComponent::RemoveGrantedPlayerWeaponAbility(UPARAM(ref)TArray<FGameplayAbilitySpecHandle> InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
		InSpecHandlesToRemove.Empty();
	}
}

bool UDynastyAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTag)
{
	check(AbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTag.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);
		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}

	}
	return false;
}
