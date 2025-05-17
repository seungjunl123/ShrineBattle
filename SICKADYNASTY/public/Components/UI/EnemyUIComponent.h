// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UDynastyWidgetBase;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UDynastyWidgetBase* InWidgetToRegister);
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidget();

private:
	TArray<UDynastyWidgetBase*> EnemyDrawnWidgets;
	
};
