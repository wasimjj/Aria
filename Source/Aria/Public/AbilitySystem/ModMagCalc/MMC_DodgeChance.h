// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_DodgeChance.generated.h"

/**
 * UMMC_DodgeChance is a class that calculates the dodge chance for a gameplay effect based on the target's agility attribute.
 *
 * It inherits from UGameplayModMagnitudeCalculation which is a base class for gameplay effect magnitude calculations.
 *
 * The class contains a constructor, CalculateBaseMagnitude_Implementation, and a private member AgilityDef.
 *
 * The constructor initializes the AgilityDef member by specifying the attribute to capture and the capture source.
 * RelevantAttributesToCapture is also updated with AgilityDef.
 *
 * The CalculateBaseMagnitude_Implementation method calculates the base magnitude of the dodge chance by evaluating the captured agility attribute
 * using the source and target tags from the gameplay effect.
 * It applies the formula: Dodge Chance = (Agility / DodgeChanceConvRate) + BaseDodgePercent, where DodgeChanceConvRate, BaseDodgePercent,
 * and DodgeChanceMaxCap are constant values.
 * DodgeChanceResult is clamped between BaseDodgePercent and DodgeChanceMaxCap to ensure it stays within the defined range.
 *
 * Note: This documentation contains code comments from the source files MMC_DodgeChance.h and MMC_DodgeChance.cpp for reference.
 * The AriaAttributeSet.h file is referenced but not shown in this documentation.
 */
UCLASS()
class ARIA_API UMMC_DodgeChance : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_DodgeChance();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition AgilityDef;
};
