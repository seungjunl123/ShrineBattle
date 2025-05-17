// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combo/AttackComboAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayCueFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "DynastyFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "KwangGameplayTags.h"
#include "SICKADYNASTY/PlayerCharacter.h"
#include "SICKADYNASTY/ComboActionData.h"

void UAttackComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!TargetCharacter)
    {
        return;
    }

    TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    UAbilityTask_PlayMontageAndWait* PlayAttackTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,  // ���� �����Ƽ(��!)
            "Attack", // TaskInstanceName, AT���� �����ϱ� ���� ������
            AttackComboMontage // �����ϴ� ��Ÿ��
        );

    PlayAttackTask->OnCompleted.AddDynamic(this, &UAttackComboAbility::OnCompleteCallback);
    PlayAttackTask->OnInterrupted.AddDynamic(this, &UAttackComboAbility::OnInterruptedCallback);
    PlayAttackTask->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent* WaitEventTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KwangGameplayTags::Shared_Event_MeleeHit);
    WaitEventTask->EventReceived.AddDynamic(this, &UAttackComboAbility::OnGameplayEventReceived);
    WaitEventTask->ReadyForActivation();

    LastInputTime = GetWorld()->GetTimeSeconds();
    SetComboTimer();
}

void UAttackComboAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);


}

void UAttackComboAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!TargetCharacter) return;

    TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);


    UDynastyFunctionLibrary::RemoveGameplayTagToActorIfFound(ActorInfo->AvatarActor.Get(), KwangGameplayTags::Player_State_HeavyAttack);

    CurrentComboCount = 0;
    bHasComboInput = false;
}

void UAttackComboAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    // ���� �Է� ���� (��: 0.1�� �̳��� �ߺ� �Է� ����)
    float CurrentTime = GetWorld()->GetTimeSeconds();

    if (CurrentTime - LastInputTime < 0.1f)
    {
        return;
    }
    LastInputTime = CurrentTime;

    if (!ComboTimerHandle.IsValid())
    {
        bHasComboInput = false;
    }
    else
    {
        bHasComboInput = true;
    }
}

void UAttackComboAbility::OnGameplayEventReceived(const FGameplayEventData Payload)
{   
    // ���������� ����
    FGameplayEffectSpecHandle SpecHandle = MakeDamageEffectSpecHandle(
        DealDemageEffect, // EffectClass
        AttackComboData->BaseDamage, // BaseDamage
        KwangGameplayTags::Player_SetByCaller_AttackType_Light, // Current Attack Tag
        CurrentComboCount // Current Combo Count
    );

    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(static_cast<AActor*>(Payload.Target));
    
    // ������ ����
    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *SpecHandle.Data,
        TargetASC
    );

    // �´� ��Ÿ�� ����
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        static_cast<AActor*>(Payload.Target),
        KwangGameplayTags::Shared_Event_HitReact,
        Payload
    );
}

void UAttackComboAbility::OnCompleteCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false; // ���� ��ҵ� ���� ����, Complete�� false

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}


void UAttackComboAbility::OnInterruptedCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = true;

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UAttackComboAbility::ComboCheck()
{
    ComboTimerHandle.Invalidate();

    if (bHasComboInput)
    {
        CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, AttackComboData->MaxComboCount);
        APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
        
        if (!TargetCharacter) return;
        UAnimInstance* AnimInstance = TargetCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            // ���� ���� �̸�
            FName SectionName = *FString::Printf(TEXT("%s%d"), *AttackComboData->SectionPrefix, CurrentComboCount + 1);

            // ������ �����ϸ� ���
            AnimInstance->Montage_JumpToSection(SectionName);
            LastInputTime = GetWorld()->GetTimeSeconds();
            SetComboTimer();
            bHasComboInput = false;
        }
        else {
            UE_LOG(LogTemp, Display, TEXT("AnimInstance is NULL"));
        }
    }
}

void UAttackComboAbility::SetComboTimer()
{
    if (AttackComboData->ComboFrame.IsValidIndex(CurrentComboCount))
    {
        // �̼���
        const float AttackSpeedRate = 1.0f;

        float ComboAvailableTime = (AttackComboData->ComboFrame[CurrentComboCount] / AttackComboData->FrameRate) / AttackSpeedRate;

        if (ComboAvailableTime > 0.0f)
        {
            GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UAttackComboAbility::ComboCheck, ComboAvailableTime, false);
        }
    }
}
