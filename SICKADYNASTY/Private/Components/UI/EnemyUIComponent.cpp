// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/DynastyWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UDynastyWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidget()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UDynastyWidgetBase* EnemyDrawnWidget : EnemyDrawnWidgets)
	{
		EnemyDrawnWidget->RemoveFromParent();
	}
}
