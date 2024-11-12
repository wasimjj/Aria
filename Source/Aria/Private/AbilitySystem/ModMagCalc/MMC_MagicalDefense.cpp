// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_MagicalDefense.h"

#include "AbilitySystem/AriaAttributeSet.h"

UMMC_MagicalDefense::UMMC_MagicalDefense()
{
	DefenseDef.AttributeToCapture = UAriaAttributeSet::GetDefenseAttribute();
	DefenseDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenseDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DefenseDef);

	SpiritDef.AttributeToCapture = UAriaAttributeSet::GetSpiritAttribute();
	SpiritDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	SpiritDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(SpiritDef);
}

float UMMC_MagicalDefense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float DefenseMagnitude = 0.f;
	float SpiritMagnitude = 0.f;

	GetCapturedAttributeMagnitude(DefenseDef, Spec, EvaluationParameters, DefenseMagnitude);
	GetCapturedAttributeMagnitude(SpiritDef, Spec, EvaluationParameters, SpiritMagnitude);

	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	DefenseMagnitude = FMath::Max<float>(DefenseMagnitude, 0);
	SpiritMagnitude = FMath::Max<float>(SpiritMagnitude, 0);
#pragma endregion

	// Formula for Calc
	const float MagicalDefenseResult = FMath::Floor(DefenseMagnitude + SpiritMagnitude);

	return MagicalDefenseResult;
}
