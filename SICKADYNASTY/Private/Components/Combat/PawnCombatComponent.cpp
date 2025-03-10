// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "SICKADYNASTY/BaseCharacter.h"


void UPawnCombatComponent::DelegateBindingToActor()
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	if (BaseCharacter)
	{
		BaseCharacter->OnAttackHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	}
}

UPawnCombatComponent::UPawnCombatComponent()
{
	DelegateBindingToActor();
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}