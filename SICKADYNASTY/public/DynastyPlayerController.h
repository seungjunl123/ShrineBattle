// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "DynastyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SICKADYNASTY_API ADynastyPlayerController : public APlayerController ,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	ADynastyPlayerController();

	// IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId PlayerTeamId;
};
