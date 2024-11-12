// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Attack.generated.h"

/**
 * UMMC_Attack is a class that calculates the base magnitude of gameplay effects related to attack abilities.
 *
 * This class is derived from the UGameplayModMagnitudeCalculation class, which is a base class for making gameplay effect magnitudes calculations.
 *
 * Public Functions:
 * - UMMC_Attack() - Constructor for UMMC_Attack objects.
 * - float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const - Overrides the CalculateBaseMagnitude_Implementation method from the UGameplayModMagnitudeCalculation class and calculates the base magnitude of the gameplay effect.
 *
 * Private Variables:
 * - FGameplayEffectAttributeCaptureDefinition StrengthDef - Stores information about the Strength attribute capture definition.
 *
 * Usage:
 * 1. Create a UMMC_Attack object.
 * 2. Call the CalculateBaseMagnitude_Implementation method passing in the FGameplayEffectSpec of the gameplay effect that needs the base magnitude to be calculated.
 * 3. The method will return the calculated base magnitude of the gameplay effect.
 *
 * Note: This class requires AriaAttributeSet.h to be included.
 */
UCLASS()
class ARIA_API UMMC_Attack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Attack();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition StrengthDef;
};
