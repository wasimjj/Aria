// Copyright Savoie Interactive Studios


#include "AbilitySystem/EffectContexts/AriaStatusEffectContext.h"

/*
 * IMPORTANT: This applies to anything that needs to be handled in the Status Effect Context
 */
bool FAriaStatusEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	// Check if the Archive is saving
	if (Ar.IsSaving())
	{
		// Flip some bits
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		// Next Bits used for the custom property variables
		/* Status Effect Core Properties */
		if (bIsStatusEffectApplied)
		{
			RepBits |= 1 << 7;
		}
		if (StatusEffectType.IsValid())
		{
			RepBits |= 1 << 8;
		}
		if (StatusEffectDuration > 0.f)
		{
			RepBits |= 1 << 9;
		}
		if (StatusEffectFrequency > 0.f)
		{
			RepBits |= 1 << 10;
		}
		/* Status Effect Damage */
		if (DamagedAttributeType.IsValid())
		{
			RepBits |= 1 << 11;
		}
		if (DamagedAttributeAmount > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (AdditionalDamageTrigger != EAdditionalDamageTriggers::Disabled)
		{
			RepBits |= 1 << 13;
		}
		if (AdditionalDamageAmount > 0.f)
		{
			RepBits |= 1 << 14;
		}
		/* Action Hindering */
		if (RandomlyMissActions != ERandomlyMissAnyActions::Disabled)
		{
			RepBits |= 1 << 15;
		}
		if (RandomMissPercentage > 0.f)
		{
			RepBits |= 1 << 16;
		}
		if (RandomlyStopActions != ERandomlyStopAnyActions::Disabled)
		{
			RepBits |= 1 << 17;
		}
		if (ReducedActionPercentage > 0.f)
		{
			RepBits |= 1 << 18;
		}
		if (StopAllActions != EStopAllActions::Disabled)
		{
			RepBits |= 1 << 19;
		}
		/* Movement Hindering */
		if (ReducedMovementSpeed > 0.f)
		{
			RepBits |= 1 << 20;
		}
		if (bRandomizeMovements)
		{
			RepBits |= 1 << 21;
		}
		if (bRunFromTarget)
		{
			RepBits |= 1 << 22;
		}
		/* Other */
		if (bInstantKillTarget)
		{
			RepBits |= 1 << 23;
		}
		if (ReducedLevels > 0.f)
		{
			RepBits |= 1 << 24;
		}
		if (bForceAIControl)
		{
			RepBits |= 1 << 25;
		}
	}

	// LengthBits is always one extra than the total above; up to 32 max
	Ar.SerializeBits(&RepBits, 26);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = MakeShared<FHitResult>();
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	// Next Bits used for the custom property variables
	
	/* Status Effect Core Properties */
	if (RepBits & (1 << 7))
	{
		Ar << bIsStatusEffectApplied;
	}
	if (RepBits & (1 << 8))
	{
		if (Ar.IsLoading())
		{
			if (!StatusEffectType.IsValid())
			{
				StatusEffectType = MakeShared<FGameplayTag>();
			}
		}
		StatusEffectType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 9))
	{
		Ar << StatusEffectDuration;
	}
	if (RepBits & (1 << 10))
	{
		Ar << StatusEffectFrequency;
	}
	
	/* Status Effect Damage */
	if (RepBits & (1 << 11))
	{
		if (Ar.IsLoading())
		{
			if (!DamagedAttributeType.IsValid())
			{
				DamagedAttributeType = MakeShared<FGameplayTag>();
			}
		}
		DamagedAttributeType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 12))
	{
		Ar << DamagedAttributeAmount;
	}
	if (RepBits & (1 << 13))
	{
		uint8 AdditDamageTrigger = static_cast<uint8>(AdditionalDamageTrigger);
		Ar << AdditDamageTrigger;
	}
	if (RepBits & (1 << 14))
	{
		Ar << AdditionalDamageAmount;
	}
	
	/* Action Hindering */
	if (RepBits & (1 << 15))
	{
		uint8 RandomMissActions = static_cast<uint8>(RandomlyMissActions);
		Ar << RandomMissActions;
	}
	if (RepBits & (1 << 16))
	{
		Ar << RandomMissPercentage;
	}
	if (RepBits & (1 << 17))
	{
		uint8 RandomStopActions = static_cast<uint8>(RandomlyStopActions);
		Ar << RandomStopActions;
	}
	if (RepBits & (1 << 18))
	{
		Ar << ReducedActionPercentage;
	}
	if (RepBits & (1 << 19))
	{
		uint8 StopActions = static_cast<uint8>(StopAllActions);
		Ar << StopActions;
	}

	/* Movement Hindering */
	if (RepBits & (1 << 20))
	{
		Ar << ReducedMovementSpeed;
	}
	if (RepBits & (1 << 21))
	{
		Ar << bRandomizeMovements;
	}
	if (RepBits & (1 << 22))
	{
		Ar << bRunFromTarget;
	}

	/* Other */
	if (RepBits & (1 << 23))
	{
		Ar << bInstantKillTarget;
	}
	if (RepBits & (1 << 24))
	{
		Ar << ReducedLevels;
	}
	if (RepBits & (1 << 25))
	{
		Ar << bForceAIControl;
	}
	
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;

	return true;
}
