// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_MagicalAttack.h"

#include "AbilitySystem/AriaAttributeSet.h"

UMMC_MagicalAttack::UMMC_MagicalAttack()
{
	IntelligenceDef.AttributeToCapture = UAriaAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MagicalAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float IntelligenceMagnitude = 0.f;
	// This sets the Magnitude for the float AttributeMag specified above.
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, IntelligenceMagnitude);
	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	IntelligenceMagnitude = FMath::Max<float>(IntelligenceMagnitude, 0);
#pragma endregion

	// Magical Attack Base Value
	constexpr float BaseMagicalAttack = 25.f;

	// Intelligence Buffer
	//  This is a small number added so that the Logarithm formula below works even with a low Attribute value
	constexpr float IntelligenceBuffer = 10.f;

	// Formula for Calc
	const float MagicalAttackResult = (IntelligenceMagnitude + IntelligenceBuffer) * FMath::Log2(
		IntelligenceMagnitude + IntelligenceBuffer) + BaseMagicalAttack;

	return MagicalAttackResult;
}
