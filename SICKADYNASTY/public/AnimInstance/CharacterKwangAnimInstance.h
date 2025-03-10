// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/CharacterBaseAnimInstance.h"
#include "CharacterKwangAnimInstance.generated.h"


class ABaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UCharacterKwangAnimInstance : public UCharacterBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category ="AnimData|Refrences")
	ABaseCharacter* OwningCharacter; 
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category ="AnimData|LocomotionData")
	float GroundSpeed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category ="AnimData|LocomotionData")
	bool bHasAccelation;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};

