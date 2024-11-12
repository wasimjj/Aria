// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AriaAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDef.AttributeToCapture = UAriaAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
#pragma region CapturedAttribute_GetDetails
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// These are the Parameters that we have to pass in to a specific Function in order to capture the Attribute we are interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float VitalityMagnitude = 0.f;
	// This sets the Magnitude for the float AttributeMag specified above.
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParameters, VitalityMagnitude);
	// If Attribute is less than 0, then this auto sets it to 0; this prevents negative values
	VitalityMagnitude = FMath::Max<float>(VitalityMagnitude, 0);
#pragma endregion

	// Get the CharacterLevel and store it
	// Note: The GetSourceObject in this case it going to be the Aria Character which has the CombatInterface implemented.
	int32 CharacterLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	// Max Health Modifier
	constexpr float MaxHealthModifier = 16.5f;

	// Formula for Calc
	const float MaxHealthResult = VitalityMagnitude * MaxHealthModifier * CharacterLevel;

	// Returns the Formula to use for the Modifer Calculation 
	return MaxHealthResult;
}
