// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Containers/UnrealString.h"
#include "Templates/SharedPointer.h"
#include "AriaMainDamageEffectContext.generated.h"

/**
 * Structure representing the main damage effect context of a gameplay effect in the AriaAbilitySystem.
 *
 * This struct extends the FGameplayEffectContext class and adds additional properties
 * and methods specific to the AriaAbilitySystem.
 */
USTRUCT(BlueprintType)
struct FAriaMainDamageEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    // Getters for the Main Damage Effect Context
    /* Damage Checks */
    bool IsDodgedHit() const { return bIsDodgedHit; }
    bool IsBlockedHit() const { return bIsBlockedHit; }
    bool IsCriticalHit() const { return bIsCriticalHit; }
    /* Base Damage */
    TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
    /* Splash (Radial) Damage */
    bool IsRadialDamage() const { return bIsRadialDamage; }
    float GetRadialDamageMinimumAmt() const { return RadialDamageMinimumAmt; }
    float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
    float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
    FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
    /* Pushback Effects */
    FVector GetKnockbackForce() const { return KnockbackForce; }
    FVector GetDeathImpulse() const { return DeathImpulse; }
    
    // Setters for the Main Damage Effect Context
    /* Damage Checks */
    void SetIsDodgedHit(const bool bInIsDodgedHit) { bIsDodgedHit = bInIsDodgedHit; }
    void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
    void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
    /* Base Damage */
    void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
    /* Splash (Radial) Damage */
    void SetIsRadialDamage(const bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
    void SetRadialDamageMinimumAmt(const float InRadialDamageMinimumAmt) { RadialDamageMinimumAmt = InRadialDamageMinimumAmt; }
    void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
    void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
    void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
    /* Pushback Effects */
    void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
    void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }

    bool bIsAriaMainDamageEffectContext = true;
    
    /** Returns the original struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return StaticStruct();
    }
    
    /** Creates a copy of this context, used to duplicate for later modifications */
    virtual FAriaMainDamageEffectContext* Duplicate() const override
    {
        FAriaMainDamageEffectContext* NewContext = new FAriaMainDamageEffectContext();
        *NewContext = *this;
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }
    
    /** Custom serialization, subclasses must override this */
    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
    
    /* Damage Checks */
    UPROPERTY()
    bool bIsDodgedHit = false;

    UPROPERTY()
    bool bIsBlockedHit = false;

    UPROPERTY()
    bool bIsCriticalHit = false;

    /* Base Damage */
    TSharedPtr<FGameplayTag> DamageType;

    /* Splash (Radial) Damage */
    UPROPERTY()
    bool bIsRadialDamage = false;

    UPROPERTY()
    float RadialDamageMinimumAmt = 0.f;
	
    UPROPERTY()
    float RadialDamageInnerRadius = 0.f;

    UPROPERTY()
    float RadialDamageOuterRadius = 0.f;

    UPROPERTY()
    FVector RadialDamageOrigin = FVector::ZeroVector;

    /* Pushback Effects */
    UPROPERTY()
    FVector KnockbackForce = FVector::ZeroVector;

    UPROPERTY()
    FVector DeathImpulse = FVector::ZeroVector;
};

template <>
struct TStructOpsTypeTraits<FAriaMainDamageEffectContext> : TStructOpsTypeTraitsBase2<FAriaMainDamageEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};
