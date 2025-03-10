// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCheckNotifyState.h"
#include "BaseCharacter.h"

void UAttackCheckNotifyState::NotifyBegin(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float TotalDuration, const FAnimNotifyEventReference &EventReference)
{
}

void UAttackCheckNotifyState::NotifyTick(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float FrameDeltaTime, const FAnimNotifyEventReference &EventReference)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
        if (Character)
        {
            Character->PerformAttackTrace(BaseLoc, TipLoc);
        }
    }
}

void UAttackCheckNotifyState::NotifyEnd(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
        if (MeshComp && MeshComp->GetOwner())
    {
        ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
        if (Character)
        {
            Character->ResetAttack();
        }
    }
}
