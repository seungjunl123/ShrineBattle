// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/CharacterBaseAnimInstance.h"
#include "CharacterLinkedAnimLayer.generated.h"

class UCharacterKwangAnimInstance;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UCharacterLinkedAnimLayer : public UCharacterBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	UCharacterKwangAnimInstance* GetPlayerAnimInstance() const;
};
