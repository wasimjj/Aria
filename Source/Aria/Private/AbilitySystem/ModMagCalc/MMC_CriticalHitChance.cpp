// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_CriticalHitChance.h"

#include "AbilitySystem/AriaAttributeSet.h"

UMMC_CriticalHitChance::UMMC_CriticalHitChance()
{
	DexterityDef.AttributeToCapture = UAriaAttributeSet::GetDexterityAttribute();
	DexterityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DexterityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DexterityDef);

	AgilityDef.AttributeToCapture = UAriaAttributeSet::GetAgilityAttribute();
	AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);
}

float UMMC_CriticalHitChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float DexterityMagnitude = 0.f;
	float AgilityMagnitude = 0.f;

	GetCapturedAttributeMagnitude(DexterityDef, Spec, EvaluationParameters, DexterityMagnitude);
	GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, AgilityMagnitude);

	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	DexterityMagnitude = FMath::Max<float>(DexterityMagnitude, 0);
	AgilityMagnitude = FMath::Max<float>(AgilityMagnitude, 0);
#pragma endregion

	// Base Critical Hit Chance
	constexpr float BaseCriticalHitChance = 5.f;

	// Critical Hit Chance Modifier
	constexpr float CriticalHitChanceModifier = 10.f;

	// Critical Hit Chance Min/Max Cap
	constexpr float CriticalHitChanceMaxCap = 50.f;

	// Formula for Calc
	float CriticalHitChanceResult = FMath::Floor(
		BaseCriticalHitChance + (DexterityMagnitude - AgilityMagnitude) * CriticalHitChanceModifier);

	CriticalHitChanceResult = FMath::Clamp(CriticalHitChanceResult, BaseCriticalHitChance, CriticalHitChanceMaxCap);

	return CriticalHitChanceResult;
}
