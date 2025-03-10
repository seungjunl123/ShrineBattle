// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DynastyGameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"

class AEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UEnemyGameplayAbility : public UDynastyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Dynasty|Ability")
	AEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Dynasty|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "Dynasty|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, FScalableFloat InDamageScalableFloat);


private:
	TWeakObjectPtr<AEnemyCharacter> CachedEnemyCharacter;
};
