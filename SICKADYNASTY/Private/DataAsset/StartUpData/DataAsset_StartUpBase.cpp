// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUpData/DataAsset_StartUpBase.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/DynastyGameplayAbility.h"

void UDataAsset_StartUpBase::GiveToAbilitySystemComponent(UDynastyAbilitySystemComponent *InASCToGive, int32 AbilityLevel)
{
    check(InASCToGive);

    GrantAbilities(ActivateOnGivenAbilities,InASCToGive,AbilityLevel);
    GrantAbilities(ReactiveAbilities,InASCToGive,AbilityLevel);

    if (!StartUpGameplayEffects.IsEmpty())
    {
        for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
        {
            if (!EffectClass) continue;

            UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
            InASCToGive->ApplyGameplayEffectToSelf(
                EffectCDO,
                AbilityLevel,
                InASCToGive->MakeEffectContext()
            );
        }
    }
}

void UDataAsset_StartUpBase::GrantAbilities(const TArray<TSubclassOf<UDynastyGameplayAbility>> &InAbilityToGive, UDynastyAbilitySystemComponent *InASCToGive, int32 AbilityLevel)
{
    if(InAbilityToGive.IsEmpty())
    {
        return;
    }

    for(const TSubclassOf<UDynastyGameplayAbility>& Ability : InAbilityToGive)
    {
        if(!Ability) continue;

  
        FGameplayAbilitySpec AbilitySpec(Ability);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = AbilityLevel;

        InASCToGive->GiveAbility(AbilitySpec);
    }
}
