// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Pickup/DynastyGameplayAbility_Pickup.h"
#include "SICKADYNASTY/PlayerCharacter.h"
#include "Item/DynastyStoneBase.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UDynastyGameplayAbility_Pickup::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    GetPlayerUiComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDynastyGameplayAbility_Pickup::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    GetPlayerUiComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDynastyGameplayAbility_Pickup::CollectStones()
{
    CollectedStones.Empty();

    TArray<FHitResult> TraceHits;

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetPlayerCharacterFromActorInfo(),
        GetPlayerCharacterFromActorInfo()->GetActorLocation(),
        GetPlayerCharacterFromActorInfo()->GetActorLocation() + -GetPlayerCharacterFromActorInfo()->GetActorUpVector() * BoxTraceDistance,
        TraceBoxSize / 2.f,
        (-GetPlayerCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
        StoneTraceChannel,
        false,
        TArray<AActor*>(),
        bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
        TraceHits,
        true
    );

    for (const FHitResult& TraceHit : TraceHits)
    {
        if (ADynastyStoneBase* FoundStone = Cast<ADynastyStoneBase>(TraceHit.GetActor()))
        {
            CollectedStones.AddUnique(FoundStone);
        }
    }

    if (CollectedStones.IsEmpty())
    {
        CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
    }
}

void UDynastyGameplayAbility_Pickup::ConsumeStones()
{
    if (CollectedStones.IsEmpty())
    {
        CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
        return;
    }

    for (ADynastyStoneBase* CollectedStone : CollectedStones)
    {
        if (CollectedStone)
        {
            CollectedStone->Consume(GetDynastyAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
        }
    }
}
