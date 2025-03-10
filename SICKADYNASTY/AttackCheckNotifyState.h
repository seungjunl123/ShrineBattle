// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackCheckNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UAttackCheckNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);

protected:
	UPROPERTY(EditAnywhere,  Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName BaseLoc;
	UPROPERTY(EditAnywhere,  Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName TipLoc;
};
