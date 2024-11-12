// Copyright Savoie Interactive Studios


#include "AriaAbilityTypes.h"

/*
 * IMPORTANT: This applies to anything that needs to be handled in the Effect Context
 */
bool FAriaGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
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
		if (bIsDodgedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 9;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 10;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 11;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 12;
		}
		if (bIsStatusEffectApplied)
		{
			RepBits |= 1 << 13;
		}
		if (StatusEffectType.IsValid())
		{
			RepBits |= 1 << 14;
		}
		if (StatusEffectDuration > 0.f)
		{
			RepBits |= 1 << 15;
		}
		if (StatusEffectFrequency > 0.f)
		{
			RepBits |= 1 << 16;
		}
		if (DamagedAttributeType.IsValid())
		{
			RepBits |= 1 << 17;
		}
		if (DamagedAttributeAmount > 0.f)
		{
			RepBits |= 1 << 18;
		}
		if (AdditionalDamageTrigger != EAdditionalDamageTriggers::Disabled)
		{
			RepBits |= 1 << 19;
		}
		if (AdditionalDamageAmount > 0.f)
		{
			RepBits |= 1 << 20;
		}
		if (RandomlyMissActions != ERandomlyMissAnyActions::Disabled)
		{
			RepBits |= 1 << 21;
		}
		if (RandomMissPercentage > 0.f)
		{
			RepBits |= 1 << 22;
		}
		if (RandomlyStopActions != ERandomlyStopAnyActions::Disabled)
		{
			RepBits |= 1 << 23;
		}
		if (ReducedActionPercentage > 0.f)
		{
			RepBits |= 1 << 24;
		}
		if (StopAllActions != EStopAllActions::Disabled)
		{
			RepBits |= 1 << 25;
		}
		if (ReducedMovementSpeed > 0.f)
		{
			RepBits |= 1 << 26;
		}
		if (bRandomizeMovements)
		{
			RepBits |= 1 << 27;
		}
		if (bRunFromTarget)
		{
			RepBits |= 1 << 28;
		}
		if (bInstantKillTarget)
		{
			RepBits |= 1 << 29;
		}
		if (ReducedLevels > 0.f)
		{
			RepBits |= 1 << 30;
		}
		if (bForceAIControl)
		{
			RepBits |= 1 << 31;
		}
	}

	// LengthBits is always one extra than the total above; up to 32 max
	Ar.SerializeBits(&RepBits, 32);

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
	if (RepBits & (1 << 7))
	{
		Ar << bIsDodgedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 10))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = FGameplayTag();
			}
		}
		DamageType.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 11))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 12))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 13))
	{
		Ar << bIsStatusEffectApplied;
	}
	if (RepBits & (1 << 14))
	{
		if (Ar.IsLoading())
		{
			if (!StatusEffectType.IsValid())
			{
				StatusEffectType = FGameplayTag();
			}
		}
		StatusEffectType.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		Ar << StatusEffectDuration;
	}
	if (RepBits & (1 << 16))
	{
		Ar << StatusEffectFrequency;
	}
	if (RepBits & (1 << 17))
	{
		if (Ar.IsLoading())
		{
			if (!DamagedAttributeType.IsValid())
			{
				DamagedAttributeType = FGameplayTag();
			}
		}
		DamagedAttributeType.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 18))
	{
		Ar << DamagedAttributeAmount;
	}
	if (RepBits & (1 << 19))
	{
		uint8 AdditDamageTrigger = static_cast<uint8>(AdditionalDamageTrigger);
		Ar << AdditDamageTrigger;
	}
	if (RepBits & (1 << 20))
	{
		Ar << AdditionalDamageAmount;
	}
	if (RepBits & (1 << 21))
	{
		uint8 RandomMissActions = static_cast<uint8>(RandomlyMissActions);
		Ar << RandomMissActions;
	}
	if (RepBits & (1 << 22))
	{
		Ar << RandomMissPercentage;
	}
	if (RepBits & (1 << 23))
	{
		uint8 RandomStopActions = static_cast<uint8>(RandomlyStopActions);
		Ar << RandomStopActions;
	}
	if (RepBits & (1 << 24))
	{
		Ar << ReducedActionPercentage;
	}
	if (RepBits & (1 << 25))
	{
		uint8 StopActions = static_cast<uint8>(StopAllActions);
		Ar << StopActions;
	}
	if (RepBits & (1 << 26))
	{
		Ar << ReducedMovementSpeed;
	}
	if (RepBits & (1 << 27))
	{
		Ar << bRandomizeMovements;
	}
	if (RepBits & (1 << 28))
	{
		Ar << bRunFromTarget;
	}
	if (RepBits & (1 << 29))
	{
		Ar << bInstantKillTarget;
	}
	if (RepBits & (1 << 30))
	{
		Ar << ReducedLevels;
	}
	if (RepBits & (1 << 31))
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
