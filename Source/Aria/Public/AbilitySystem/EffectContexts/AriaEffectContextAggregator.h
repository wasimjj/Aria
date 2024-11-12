// Copyright Savoie Interactive Studios

#pragma once

#include "GameplayEffectTypes.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Containers/UnrealString.h"
#include "Templates/SharedPointer.h"
#include "AbilitySystem/EffectContexts/AriaMainDamageEffectContext.h"
#include "AbilitySystem/EffectContexts/AriaStatusEffectContext.h"
#include "AriaEffectContextAggregator.generated.h"

/**
 * Effect Context Aggregator for handling/combining multiple gameplay effect contexts
 * in the AriaAbilitySystem.
 */
USTRUCT(BlueprintType)
struct FAriaEffectContextAggregator : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    FAriaEffectContextAggregator() = default;

    // Getter for the Main Damage context
    TSharedPtr<FAriaMainDamageEffectContext> GetMainDamageContext() const { return MainDamageContext; }
    // Getter for the Status Effect context
    TSharedPtr<FAriaStatusEffectContext> GetStatusEffectContext() const { return StatusEffectContext; }
    
    // Setter for the Main Damage context
    void SetMainDamageContext(const TSharedPtr<FAriaMainDamageEffectContext>& InMainDamageContext) { MainDamageContext = InMainDamageContext; }
    // Setter for the Status Effect context
    void SetStatusEffectContext(const TSharedPtr<FAriaStatusEffectContext>& InStatusEffectContext) { StatusEffectContext = InStatusEffectContext; }

    /** Returns the original struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return StaticStruct();
    }

    virtual FAriaEffectContextAggregator* Duplicate() const override
    {
        FAriaEffectContextAggregator* NewContext = new FAriaEffectContextAggregator();
        *NewContext = *this;
        return NewContext;
    }

    /** Custom serialization, subclasses must override this */
    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
    TSharedPtr<FAriaMainDamageEffectContext> MainDamageContext;
    TSharedPtr<FAriaStatusEffectContext> StatusEffectContext;
};

template <>
struct TStructOpsTypeTraits<FAriaEffectContextAggregator> : TStructOpsTypeTraitsBase2<FAriaEffectContextAggregator>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};
