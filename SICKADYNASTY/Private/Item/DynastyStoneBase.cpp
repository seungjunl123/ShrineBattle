// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DynastyStoneBase.h"
#include "SICKADYNASTY/PlayerCharacter.h"
#include "AbilitySystem/DynastyAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "KwangGameplayTags.h"

void ADynastyStoneBase::Consume(UDynastyAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
    check(StoneGameplayEffectClass);

    UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

    AbilitySystemComponent->ApplyGameplayEffectToSelf(
        EffectCDO,
        ApplyLevel,
        AbilitySystemComponent->MakeEffectContext()
    );

    BP_OnStoneConsumed();
}

void ADynastyStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* OverlappedHeroCharacter = Cast<APlayerCharacter>(OtherActor))
    {
        OverlappedHeroCharacter->GetKwangAbilitySystemComponent()->TryActivateAbilityByTag(KwangGameplayTags::Player_Ability_PickUp_Stone);
    }
}
