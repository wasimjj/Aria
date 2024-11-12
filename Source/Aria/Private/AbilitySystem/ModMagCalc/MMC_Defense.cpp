// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_Defense.h"

#include "AbilitySystem/AriaAttributeSet.h"

UMMC_Defense::UMMC_Defense()
{
	ResilienceDef.AttributeToCapture = UAriaAttributeSet::GetResilienceAttribute();
	ResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResilienceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ResilienceDef);
}

float UMMC_Defense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float ResilienceMagnitude = 0.f;
	// This sets the Magnitude for the float AttributeMag specified above.
	GetCapturedAttributeMagnitude(ResilienceDef, Spec, EvaluationParameters, ResilienceMagnitude);
	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	ResilienceMagnitude = FMath::Max<float>(ResilienceMagnitude, 0);
#pragma endregion

	// Base Defense Conversion Rate
	constexpr float BaseDefenseConvRate = 1.5f;

	// Formula for Calc
	const float DefenseResult = FMath::Floor(ResilienceMagnitude * BaseDefenseConvRate);

	return DefenseResult;
}
