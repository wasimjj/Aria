// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MagicalDefense.generated.h"

/**
 * @class UMMC_MagicalDefense
 * @brief This class is a gameplay calculation class for calculating the base magnitude of a magical defense effect in the Aria game.
 */
UCLASS()
class ARIA_API UMMC_MagicalDefense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MagicalDefense();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition SpiritDef;
};
