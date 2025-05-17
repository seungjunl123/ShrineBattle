// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DynastyGameplayAbility.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "DynastyFunctionLibrary.h" 
#include "KwangGameplayTags.h"
#include "DynastyTypes/DynastyEnumTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

// 어빌리티가 부여된 직후 호출
void UDynastyGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

  
    if(AbilityActivationPolicy == EDynastyAbilityActivationPolicy::OnGiven)
    {
        if(ActorInfo && !Spec.IsActive())
        {
            ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

// 어빌리티가 종료된 직후 호출
void UDynastyGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if(AbilityActivationPolicy == EDynastyAbilityActivationPolicy::OnGiven)
    {
        if(ActorInfo)
        {
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
        }
    }
}

UPawnCombatComponent *UDynastyGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
    // FindComponentByClass로 해당 클래스 중 가장 먼저 찾아지는 컴포넌트를 리턴턴
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}
UDynastyAbilitySystemComponent *UDynastyGameplayAbility::GetDynastyAbilitySystemComponentFromActorInfo() const
{
    return Cast<UDynastyAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
    
}

FActiveGameplayEffectHandle UDynastyGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    check(TargetASC);
    check(InSpecHandle.IsValid());

    return GetDynastyAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *InSpecHandle.Data,
        TargetASC
    );
}

FActiveGameplayEffectHandle UDynastyGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EDynastySuccessType& OutSuccessType)
{
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

    OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EDynastySuccessType::Successful : EDynastySuccessType::Failed;

    return ActiveGameplayEffectHandle;
}

void UDynastyGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
    if (InHitResults.IsEmpty())
    {
        return;
    }

    APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());

    for (const FHitResult& Hit : InHitResults)
    {
        if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
        {
            if (UDynastyFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
            {
                UE_LOG(LogTemp, Display, TEXT("Damage to: %s"), *Hit.GetActor()->GetActorNameOrLabel());
                FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);

                if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
                {
                    FGameplayEventData Data;
                    Data.Instigator = OwningPawn;
                    Data.Target = HitPawn;

                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
                        HitPawn,
                        KwangGameplayTags::Shared_Event_HitReact,
                        Data
                    );
                }
            }
        }
    }
}
