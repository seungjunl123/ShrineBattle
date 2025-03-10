// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUpData/DataAsset_EnemyStartUpBase.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/EnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpBase::GiveToAbilitySystemComponent(UDynastyAbilitySystemComponent* InASCToGive, int32 AbilityLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, AbilityLevel);

	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UEnemyGameplayAbility>& AblilityClass : EnemyCombatAbilities)
		{
			if (!AblilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AblilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = AbilityLevel;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
