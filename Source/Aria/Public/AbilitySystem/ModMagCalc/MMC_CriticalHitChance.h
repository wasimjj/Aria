// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CriticalHitChance.generated.h"

/**
 * @class UMMC_CriticalHitChance
 *
 * @brief This class is responsible for calculating the base magnitude of a gameplay effect related to critical hit chance.
 *
 * The UMMC_CriticalHitChance class inherits from the UGameplayModMagnitudeCalculation class, which is a calculation class used to calculate the base magnitude of a gameplay effect.
 *
 * UMMC_CriticalHitChance provides a method named CalculateBaseMagnitude_Implementation, which takes a FGameplayEffectSpec object as a parameter and returns a float. This method calculates the base magnitude of the gameplay effect based on the captured attributes and applies a formula to determine the critical hit chance value.
 *
 * The class also contains two private member variables, DexterityDef and AgilityDef, which are of type FGameplayEffectAttributeCaptureDefinition. These member variables capture the Dexterity and Agility attributes needed for the calculation of the critical hit chance.
 *
 * @see FGameplayEffectSpec
 * @see FGameplayEffectAttributeCaptureDefinition
 */
UCLASS()
class ARIA_API UMMC_CriticalHitChance : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_CriticalHitChance();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition DexterityDef;
	FGameplayEffectAttributeCaptureDefinition AgilityDef;
};
