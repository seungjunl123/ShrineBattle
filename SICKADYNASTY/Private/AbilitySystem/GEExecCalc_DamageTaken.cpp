// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/DynastyAttributeSet.h"
#include "KwangGameplayTags.h"

// 데미지 적용에 사용할 어트리뷰트 캡쳐
struct FDynastyDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	FDynastyDamageCapture()
	{ 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDynastyAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDynastyAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDynastyAttributeSet, DamageTaken, Target, false)
	}
};

static const FDynastyDamageCapture& GetDynastyDamageCapture()
{
	static FDynastyDamageCapture DynastyDamageCapture;
	return DynastyDamageCapture;
};

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetDynastyDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetDynastyDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetDynastyDamageCapture().DamageTakenDef);
}
// 데미지 계산 및 적용
void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags(); // 공격자 태그
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags(); // 피격자 태그

	// 공격 데미지 캡쳐
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDynastyDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	float BaseDamage = 0.f;
	int32 UsedLightAttackCombo = 0;

	// 공격 데미지 설정
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(KwangGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(KwangGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackCombo = TagMagnitude.Value;
		}
	}
	// 피격자 방어력 캡쳐
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDynastyDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	// 콤보에 따른 데미지 증감
	if (UsedLightAttackCombo != 0)
	{
		const float DamageIncresePercentLight = (UsedLightAttackCombo - 1) * 0.05 + 1.f;
		BaseDamage += DamageIncresePercentLight;
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	if (FinalDamageDone > 0.f)
	{
		// Attribute 모디파이어를 통한 최종 데미지 적용
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetDynastyDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
