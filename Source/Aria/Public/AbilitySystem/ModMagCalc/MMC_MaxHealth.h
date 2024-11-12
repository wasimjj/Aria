// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * \class UMMC_MaxHealth
 * \brief A class used to calculate the maximum health modifier for a gameplay effect.
 *
 * This class inherits from UGameplayModMagnitudeCalculation and overrides the CalculateBaseMagnitude_Implementation method. It also contains a private member variable for capturing the vitality attribute.
 *
 * Usage:
 * 1. Create an instance of UMMC_MaxHealth.
 * 2. Use the instance to calculate the maximum health modifier by calling the CalculateBaseMagnitude_Implementation method and passing a FGameplayEffectSpec object.
 *
 * Example:
 * \code
 * UMMC_MaxHealth* MaxHealthCalculation = NewObject<UMMC_MaxHealth>();
 * float MaxHealthModifier = MaxHealthCalculation->CalculateBaseMagnitude_Implementation(MyGameplayEffectSpec);
 * \endcode
 */
UCLASS()
class ARIA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VitalityDef;
};
