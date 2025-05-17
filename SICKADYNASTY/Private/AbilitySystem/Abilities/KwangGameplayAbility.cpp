// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KwangGameplayAbility.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "SICKADYNASTY/PlayerCharacter.h"
#include "KwangGameplayTags.h"
#include "DynastyPlayerController.h"

APlayerCharacter *UKwangGameplayAbility::GetPlayerCharacterFromActorInfo()
{
    if(!ChachedPlayerCharacter.IsValid())
    {
        ChachedPlayerCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor);
    }
    return ChachedPlayerCharacter.IsValid()? ChachedPlayerCharacter.Get() : nullptr;
}
ADynastyPlayerController *UKwangGameplayAbility::GetPlayerControllerFromActorInfo()
{
    if(!ChachedDynastyPlayerController.IsValid())
    {
        ChachedDynastyPlayerController = Cast<ADynastyPlayerController>(CurrentActorInfo->PlayerController);
    }
    return ChachedDynastyPlayerController.IsValid()? ChachedDynastyPlayerController.Get() : nullptr;
}
UKwangCombatComponent *UKwangGameplayAbility::GetKwangCombatComponentFromActorInfo()
{
    return GetPlayerCharacterFromActorInfo()->GetKwangCombatComponent();
}

FGameplayEffectSpecHandle UKwangGameplayAbility::MakeDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentCombocount)
{
    check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetDynastyAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetDynastyAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        KwangGameplayTags::Shared_SetByCaller_BaseDamage,
        InBaseDamage
    );

    if (InCurrentAttackTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentCombocount);
    }

    return EffectSpecHandle;
}

UPlayerUIComponent* UKwangGameplayAbility::GetPlayerUiComponentFromActorInfo()
{
    return GetPlayerCharacterFromActorInfo()->GetPlayerUiComponent();
}
