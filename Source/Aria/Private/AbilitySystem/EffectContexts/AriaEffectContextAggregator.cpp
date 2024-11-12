// Copyright Savoie Interactive Studios


#include "AbilitySystem/EffectContexts/AriaEffectContextAggregator.h"

bool FAriaEffectContextAggregator::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	bOutSuccess &= MainDamageContext.IsValid() && MainDamageContext->NetSerialize(Ar, Map, bOutSuccess);
	bOutSuccess &= StatusEffectContext.IsValid() && StatusEffectContext->NetSerialize(Ar, Map, bOutSuccess);
	return bOutSuccess;
}
