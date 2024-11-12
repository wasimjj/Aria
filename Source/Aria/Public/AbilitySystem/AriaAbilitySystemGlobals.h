// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AriaAbilitySystemGlobals.generated.h"

/**
 * UAriaAbilitySystemGlobals is a subclass of UAbilitySystemGlobals that provides
 * additional functionality specific to the game.
 *
 * This class overrides the AllocGameplayEffectContext() method from UAbilitySystemGlobals
 * to allocate an instance of the FAriaEffectContextAggregator class.
 *
 * @see UAbilitySystemGlobals
 */
UCLASS()
class ARIA_API UAriaAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
