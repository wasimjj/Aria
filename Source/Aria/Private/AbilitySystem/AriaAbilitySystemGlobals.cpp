// Copyright Savoie Interactive Studios


#include "AbilitySystem/AriaAbilitySystemGlobals.h"
#include "AbilitySystem/EffectContexts/AriaEffectContextAggregator.h"

FGameplayEffectContext* UAriaAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAriaEffectContextAggregator();
}
