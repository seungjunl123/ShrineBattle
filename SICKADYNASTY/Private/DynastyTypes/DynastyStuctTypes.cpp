// Fill out your copyright notice in the Description page of Project Settings.


#include "DynastyTypes/DynastyStuctTypes.h"
#include "AbilitySystem/Abilities/KwangGameplayAbility.h"

bool FKwangAbilitySet::IsValid() const
{
    return InputTag.IsValid() && AbilityToGrant;
}