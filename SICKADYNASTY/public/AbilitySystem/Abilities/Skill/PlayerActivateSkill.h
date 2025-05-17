// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/KwangGameplayAbility.h"
#include "PlayerActivateSkill.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UPlayerActivateSkill : public UKwangGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float Cost;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float CoolDown;

protected:
	// UGameplayAbility 
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnCompleteCallback();
	UFUNCTION()
	virtual void OnGameplayEventReceived(const FGameplayEventData Payload) {};
	UFUNCTION()
	void OnInterruptedCallback();

	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	FGameplayTag LowManaCueTag;
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	FGameplayTag CoolDownCueTag;

	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ManaCostEffect;
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	FScalableFloat InDamageScalableFloat;
};
