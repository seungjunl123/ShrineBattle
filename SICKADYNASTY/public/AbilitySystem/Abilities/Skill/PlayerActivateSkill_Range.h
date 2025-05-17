// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Skill/PlayerActivateSkill.h"
#include "PlayerActivateSkill_Range.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UPlayerActivateSkill_Range : public UPlayerActivateSkill
{
	GENERATED_BODY()
	
private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADynastyProjectileBase> SkillProjectile;
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	FName SkillProjectileSocket;
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ProjectileGameplayEffectClass;
	UPROPERTY()
	class UAbilityTask_WaitGameplayEvent* SpawnEventTask;

protected:
	virtual void OnGameplayEventReceived(const FGameplayEventData Payload) override;

};
