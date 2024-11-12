// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * \class UMMC_MaxMana
 *
 * \brief A gameplay effect magnitude calculation class that calculates the base magnitude for increasing the maximum mana of a character.
 *
 * This class is a derived class of UGameplayModMagnitudeCalculation and is responsible for calculating the base magnitude of a gameplay effect that increases the maximum mana attribute of a character. It overrides the CalculateBaseMagnitude_Implementation function to perform the calculation.
 *
 * \see UGameplayModMagnitudeCalculation
 * \see FGameplayEffectSpec
 * \see FGameplayEffectAttributeCaptureDefinition
 */
UCLASS()
class ARIA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
