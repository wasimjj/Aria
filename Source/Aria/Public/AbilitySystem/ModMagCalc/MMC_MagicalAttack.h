// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MagicalAttack.generated.h"

/**
 * \class UMMC_MagicalAttack
 *
 * \brief This class calculates the base magnitude of a magical attack. It inherits from UGameplayModMagnitudeCalculation.
 *
 * This class implements the CalculateBaseMagnitude_Implementation method to calculate the base magnitude of a magical attack. It captures the intelligence attribute
 * from the gameplay effect spec and uses it in the calculation. The final result is determined by a formula that includes the intelligence attribute and a base magical attack value.
 *
 * \note This class requires the ARIA_API macro to be defined in order to be part of the public API.
 *
 * \see UGameplayModMagnitudeCalculation
 * \see FGameplayEffectSpec
 * \see FGameplayEffectAttributeCaptureDefinition
 */
UCLASS()
class ARIA_API UMMC_MagicalAttack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MagicalAttack();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
