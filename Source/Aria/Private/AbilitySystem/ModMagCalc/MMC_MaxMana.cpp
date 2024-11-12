// Copyright Savoie Interactive Studios


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AriaAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAriaAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	// Get the CharacterLevel and store it
	// Note: The GetSourceObject in this case it going to be the Aria Character which has the CombatInterface implemented.
	int32 CharacterLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	// Max Mana Modifier
	constexpr float MaxManaModifier = 8.5f;

	// Formula for Calc
	const float MaxManaResult = IntelligenceMagnitude * MaxManaModifier * CharacterLevel;

	// Returns the Formula to use for the Modifer Calculation
	return MaxManaResult;
}
