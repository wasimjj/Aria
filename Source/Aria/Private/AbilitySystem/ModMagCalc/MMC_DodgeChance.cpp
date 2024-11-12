// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_DodgeChance.h"

#include "AbilitySystem/AriaAttributeSet.h"

UMMC_DodgeChance::UMMC_DodgeChance()
{
	AgilityDef.AttributeToCapture = UAriaAttributeSet::GetAgilityAttribute();
	AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);
}

float UMMC_DodgeChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AgilityMagnitude = 0.f;
	// This sets the Magnitude for the float AttributeMag specified above.
	GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, AgilityMagnitude);
	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	AgilityMagnitude = FMath::Max<float>(AgilityMagnitude, 0);
#pragma endregion

	// Dodge Chance Base Value
	constexpr float BaseDodgePercent = 5.f;

	// Dodge Chance Conversion Rate
	constexpr float DodgeChanceConvRate = 10.f;

	// Dodge Chance Max Cap
	constexpr float DodgeChanceMaxCap = 50.f;

	// Formula for Calc
	float DodgeChanceResult = FMath::Floor(AgilityMagnitude / DodgeChanceConvRate) + BaseDodgePercent;
	DodgeChanceResult = FMath::Clamp(DodgeChanceResult, BaseDodgePercent, DodgeChanceMaxCap);

	return DodgeChanceResult;
}
