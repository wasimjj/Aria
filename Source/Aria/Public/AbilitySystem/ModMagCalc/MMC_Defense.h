// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Defense.generated.h"

/**
 * UMMC_Defense is a class that calculates the base magnitude of a gameplay effect's defense attribute.
 *
 * This class inherits from UGameplayModMagnitudeCalculation, which is a base class for
 * all gameplay effect magnitude calculations. It provides a CalculateBaseMagnitude_Implementation
 * method that must be implemented by subclasses in order to calculate the base magnitude of a
 * gameplay effect.
 *
 * This class captures the Resilience attribute from a target gameplay effect and uses it to
 * calculate the defense magnitude. The Resilience attribute is obtained using the
 * FGameplayEffectAttributeCaptureDefinition class, which specifies the attribute to capture,
 * the attribute source, and whether to capture a snapshot of the attribute.
 *
 * The CalculateBaseMagnitude_Implementation method captures the Source and Target tags from
 * the gameplay effect spec, and uses these tags as parameters to capture the Resilience attribute
 * magnitude. It sets the magnitude to 0 if it is less than 0, to prevent negative values.
 *
 * The base defense conversion rate is set to 1.5f, and is used in the formula to calculate the
 * defense result. The defense result is then returned as the base magnitude of the gameplay effect.
 *
 * Usage:
 * UMMC_Defense* DefenseCalculation = NewObject<UMMC_Defense>();
 * float BaseMagnitude = DefenseCalculation->CalculateBaseMagnitude(Spec);
 */
UCLASS()
class ARIA_API UMMC_Defense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Defense();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition ResilienceDef;
};
