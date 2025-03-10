// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SICKADYNASTY/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class UEnemyUIComponent;
class UEnemyCombatComponent;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class SICKADYNASTY_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	// UPawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	virtual void PerformAttackTrace(FName StartSocket, FName EndSocket) override;
	virtual void ResetAttack() override;

protected:
	virtual void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* EnemyHealthWidgetComponent;



private:
	void InitEnemyStartupData();


	// 공격 관련
	FVector PrevBaseLoc;
	FVector PrevTipLoc;
	TSet<AActor*> HittedActors;

	float LastInputTime = 0.0f;

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent	;}

	// UPawnUIInterface
	virtual UPawnUIComponent* GetPawnUiComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUiComponent() const override;
};
