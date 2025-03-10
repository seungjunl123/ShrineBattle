// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/KwangGameplayAbility.h"
#include "AttackComboAbility.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UAttackComboAbility : public UKwangGameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnGameplayEventReceived(const FGameplayEventData Payload);

	UFUNCTION()
	void OnInterruptedCallback();

	UPROPERTY(EditAnywhere, Category = "Combat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboActionData> AttackComboData;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackComboMontage;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DealDemageEffect;


	void ComboCheck();
	void SetComboTimer();

	FTimerHandle ComboTimerHandle;
	FTimerHandle JumpAttackTimerHandle;
	int32 CurrentComboCount;
	bool bHasComboInput = false;
	float LastInputTime = 0.0f;

};
