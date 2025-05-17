// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/PlayerActivateSkill_Range.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h" 
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "KwangGameplayTags.h"
#include "SICKADYNASTY/PlayerCharacter.h"
#include "Item/DynastyProjectileBase.h"


void UPlayerActivateSkill_Range::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    SpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KwangGameplayTags::Shared_Ability_SpawnProjectile);
    SpawnEventTask->EventReceived.AddDynamic(this, &UPlayerActivateSkill_Range::OnGameplayEventReceived);
    SpawnEventTask->ReadyForActivation();
}

void UPlayerActivateSkill_Range::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPlayerActivateSkill_Range::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UPlayerActivateSkill_Range::OnGameplayEventReceived(const FGameplayEventData Payload)
{
    APlayerCharacter* CastingCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor);
    if (!CastingCharacter || !SkillProjectile) return;

    // 소켓 위치와 방향
    const FVector SocketLocation = CastingCharacter->GetMesh()->GetSocketLocation(SkillProjectileSocket);
    const FRotator CharacterRotation = UKismetMathLibrary::MakeRotFromX(CastingCharacter->GetActorForwardVector());
    const FTransform SpawnTransform = FTransform(CharacterRotation, SocketLocation);


    // 데미지스펙 설정
    FGameplayEffectSpecHandle SpecHandle = MakeDamageEffectSpecHandle(
        ProjectileGameplayEffectClass, // EffectClass
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()), // BaseDamage
        KwangGameplayTags::Player_SetByCaller_Skill_R, // Current Attack Tag
        GetAbilityLevel()
    );

    // Defer 스폰
    ADynastyProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ADynastyProjectileBase>(
        SkillProjectile,
        SpawnTransform,
        CastingCharacter,
        CastingCharacter,
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
    );


    // Setter로 EffectSpec 설정
    if (SpawnedProjectile)
    {
        SpawnedProjectile->SetProjectileDamageEffectSpecHandle(SpecHandle);
        SpawnedProjectile->FinishSpawning(SpawnTransform);
    }
}

