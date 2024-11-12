// Copyright Savoie Interactive Studios


#include "AbilitySystem/EffectContexts/AriaMainDamageEffectContext.h"

/*
 * IMPORTANT: This applies to anything that needs to be handled in the Main Damage Effect Context
 */
bool FAriaMainDamageEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
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
		/* Damage Checks */
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
		/* Base Damage */
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 10;
		}
		/* Splash (Radial) Damage */
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 11;
		}
		if (RadialDamageMinimumAmt > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (RadialDamageInnerRadius > 0.f)
		{
			RepBits |= 1 << 13;
		}
		if (RadialDamageOuterRadius > 0.f)
		{
			RepBits |= 1 << 14;
		}
		if (!RadialDamageOrigin.IsZero())
		{
			RepBits |= 1 << 15;
		}
		/* Pushback Effects */
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 16;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 17;
		}
	}

	// LengthBits is always one extra than the total above; up to 32 max
	Ar.SerializeBits(&RepBits, 18);

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
	/* Damage Checks */
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
	/* Base Damage */
	if (RepBits & (1 << 10))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = MakeShared<FGameplayTag>();
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	/* Splash (Radial) Damage */
	if (RepBits & (1 << 11))
	{
		Ar << bIsRadialDamage;
	}
	if (RepBits & (1 << 12))
	{
		Ar << RadialDamageMinimumAmt;
	}
	if (RepBits & (1 << 13))
	{
		Ar << RadialDamageInnerRadius;;
	}
	if (RepBits & (1 << 14))
	{
		Ar << RadialDamageOuterRadius;;
	}
	if (RepBits & (1 << 15))
	{
		RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
	}
	/* Pushback Effects */
	if (RepBits & (1 << 16))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 17))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;

	return true;
}
