// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUpData/DataAsset_KwangStartUpData.h"
#include "AbilitySystem/Abilities/KwangGameplayAbility.h"
#include "DynastyTypes/DynastyStuctTypes.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"

void UDataAsset_KwangStartUpData::GiveToAbilitySystemComponent(UDynastyAbilitySystemComponent *InASCToGive, int32 AbilityLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive,AbilityLevel);

    for(const FKwangAbilitySet& AbilitySet : KwangStartupAbilitySets)
    {
        if(!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec  AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = AbilityLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        InASCToGive->GiveAbility(AbilitySpec);
    }
}