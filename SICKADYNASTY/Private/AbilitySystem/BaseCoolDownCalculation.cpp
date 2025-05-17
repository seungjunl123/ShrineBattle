// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseCoolDownCalculation.h"

#include "AbilitySystem/Abilities/Skill/PlayerActivateSkill.h"
float UBaseCoolDownCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UPlayerActivateSkill* Ability = Cast<UPlayerActivateSkill>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}


	return Ability->CoolDown;
}
