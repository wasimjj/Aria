// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_Attack.h"

#include "AbilitySystem/AriaAttributeSet.h"

UMMC_Attack::UMMC_Attack()
{
	StrengthDef.AttributeToCapture = UAriaAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_Attack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float StrengthMagnitude = 0.f;
	// This sets the Magnitude for the float AttributeMag specified above.
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, StrengthMagnitude);
	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	StrengthMagnitude = FMath::Max<float>(StrengthMagnitude, 0);
#pragma endregion

	// Base Attack Value
	constexpr float BaseAttackValue = 2.f;

	// Formula for Calc
	const float AttackResult = FMath::Floor(StrengthMagnitude * BaseAttackValue);

	return AttackResult;
}
