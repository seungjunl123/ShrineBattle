// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/PlayerActivateSkill.h"
#include "AbilitySystem/DynastyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h" 
#include "KwangGameplayTags.h"
#include "Components/UI/PlayerUIComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/DynastyProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayEffect.h"
#include "SICKADYNASTY/PlayerCharacter.h"


bool UPlayerActivateSkill::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bResult = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
    if (!bResult)
    {
        ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(CoolDownCueTag);
    }

    return bResult;
}

bool UPlayerActivateSkill::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bResult = Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
    if (!bResult)
    {
        ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(LowManaCueTag);
    }

    return bResult;
}

void UPlayerActivateSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!TargetCharacter)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }


    TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    

    UAbilityTask_PlayMontageAndWait* SkillTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,  // 소유 어빌리티(나!)
            "PlayerSkill", // TaskInstanceName, AT들을 구별하기 위한 인자자
            SkillMontage // 실행하는 몽타주
        );
    SkillTask->OnCompleted.AddDynamic(this, &UPlayerActivateSkill::OnCompleteCallback);
    SkillTask->OnInterrupted.AddDynamic(this, &UPlayerActivateSkill::OnInterruptedCallback);
    SkillTask->ReadyForActivation();

    CommitAbility(Handle, ActorInfo, ActivationInfo);
    ActorInfo->AvatarActor->GetComponentByClass<UPlayerUIComponent>()->OnAbilityCooldownBegin.Broadcast(KwangGameplayTags::InputTag_Skill_R, CoolDown, CoolDown);
}

void UPlayerActivateSkill::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!TargetCharacter) return;

    TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPlayerActivateSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!TargetCharacter) return;

    TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UPlayerActivateSkill::OnCompleteCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false; // 강제 취소된 건지 여부, Complete는 false

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}



void UPlayerActivateSkill::OnInterruptedCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = true;

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
