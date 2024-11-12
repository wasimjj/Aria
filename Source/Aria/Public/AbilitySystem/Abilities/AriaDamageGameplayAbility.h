// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AriaAbilityTypes.h"
#include "AbilitySystem/Abilities/AriaGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AriaDamageGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FStatusEffectDuration
{
	GENERATED_BODY()

	// The Minimum Duration that the Status Effect should last
	//  Note: When the Effect Min and Max are set to different values, a Random duration will occur
	UPROPERTY(EditDefaultsOnly)
	float StatusEffectDurationMin = 0.f;

	// The Maximum Duration that the Status Effect should last
	//  Note: When the Effect Min and Max are set to different values, a Random duration will occur
	UPROPERTY(EditDefaultsOnly)
	float StatusEffectDurationMax = 0.f;
};

USTRUCT(BlueprintType)
struct FDamageAttribute
{
	GENERATED_BODY()

	// Determines the Attribute to apply Damage to
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttributeType = FGameplayTag();

	// The reduction amount to apply to that Attribute
	UPROPERTY(EditDefaultsOnly)
	float AttributeDamageAmount = 0.f;
};

USTRUCT(BlueprintType)
struct FAdditionalDamageTrigger
{
	GENERATED_BODY()

	// Target will take Additional Damage / (DoT) based on certain actions taken
	UPROPERTY(EditDefaultsOnly)
	EAdditionalDamageTriggers AdditionalDamageTrigger = EAdditionalDamageTriggers::Disabled;

	// The Amount of Additional Damage that will be caused
	UPROPERTY(EditDefaultsOnly)
	float AdditionalDamageAmount = 0.f;
};

USTRUCT(BlueprintType)
struct FRandomlyMissActions
{
	GENERATED_BODY()

	// Target's Attack/Ability chance-to-hit will be reduced by a certain percentage
	UPROPERTY(EditDefaultsOnly)
	ERandomlyMissAnyActions RandomlyMiss = ERandomlyMissAnyActions::Disabled;

	// Random Miss Percentage
	// (Higher values mean more Actions missed)
	UPROPERTY(EditDefaultsOnly,
		meta = (DisplayName = "Random Miss %", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float RandomMissPercentage = 0.f;
};

USTRUCT(BlueprintType)
struct FRandomlyStopActions
{
	GENERATED_BODY()

	// Target will sometimes be unable to Attack or use Abilities when set
	//  Note: This doesn't affect Target Movement
	UPROPERTY(EditDefaultsOnly)
	ERandomlyStopAnyActions RandomlyStopActions = ERandomlyStopAnyActions::Disabled;

	// Reduced Action Percentage
	// (Higher values mean more Actions stopped)
	UPROPERTY(EditDefaultsOnly,
		meta = (DisplayName = "Reduced Action %", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float ReducedActionPercentage = 0.f;
};

USTRUCT(BlueprintType)
struct FSplashDamage
{
	GENERATED_BODY()

	// The Sphere (Area of Influence) where enemies caught in that area take additional Damage
	UPROPERTY(EditDefaultsOnly)
	float SplashDamageRadius = 0.f;

	// The amount of Splash Damage to apply
	UPROPERTY(EditDefaultsOnly)
	float SplashDamageAmount = 0.f;
};

USTRUCT(BlueprintType)
struct FStatusEffect
{
	GENERATED_BODY()

	// The Type of Status Effect to Apply
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StatusEffectType;

	// The chance % that the Status Effect will be applied
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float StatusEffectChance = 0.f;

	// Status Effect Duration (Can be a set value or have a random length of time)
	UPROPERTY(EditDefaultsOnly)
	FStatusEffectDuration StatusEffectDuration;

	// How often the Status Effect should trigger
	UPROPERTY(EditDefaultsOnly)
	float StatusEffectFrequency = 0.f;

	// Causes Damage (value reductions) to Attributes
	UPROPERTY(EditDefaultsOnly)
	FDamageAttribute DamageAttribute;

	// Causes Additional Damage from Actions taken
	UPROPERTY(EditDefaultsOnly)
	FAdditionalDamageTrigger AdditionalDamageTriggers;

	// Causes Random Missing (no damage/effect applied) for Actions taken
	UPROPERTY(EditDefaultsOnly)
	FRandomlyMissActions RandomlyMissActions;

	// Causes Random Stopping (failing to Activate) for Actions taken
	UPROPERTY(EditDefaultsOnly)
	FRandomlyStopActions RandomlyStopActions;

	// Target can't Attack or use Abilities when set
	//  Note: This doesn't affect Target Movement
	UPROPERTY(EditDefaultsOnly)
	EStopAllActions StopAllActions = EStopAllActions::Disabled;

	// Target Movement Speed (Velocity) is reduced by a certain percentage
	//   Note: The Target must have a Movement Component set up for this to work
	//  Setting the value at 100 will completely stop their movement (Root them in place)
	UPROPERTY(EditDefaultsOnly,
		meta = (DisplayName = "Reduced Movement %", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"
		))
	float ReduceMovementSpeed = 0.f;

	// Target will move around in random directions opposite/near-opposite from original movement specified
	//   Note: The Target must have a Movement Component set up for this to work
	UPROPERTY(EditDefaultsOnly)
	bool RandomizeMovements = false;

	// Target will turn and run from the Target Instigator of the Ability (Ex: Causing Fear)
	//   Note: The Target must have a Movement Component set up for this to work
	UPROPERTY(EditDefaultsOnly)
	bool RunFromTarget = false;

	// Target is instantly killed when hit
	UPROPERTY(EditDefaultsOnly)
	bool InstantKillTarget = false;

	// Target's current level is reduced by a set amount
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "99", UIMin = "0", UIMax = "99"))
	int32 ReduceLevels = 0;

	// Allows AI to take over a Target's Movements and Actions (likely through a custom Behavior Tree)
	UPROPERTY(EditDefaultsOnly)
	bool ForceAIControl = false;
};


/**
 * \class UAriaDamageGameplayAbility
 *
 * \brief The UAriaDamageGameplayAbility class defines a gameplay ability that causes damage to a target actor.
 *
 * This class inherits from UAriaGameplayAbility and provides functionality to cause damage to a target actor by looping through all the damage types in the DamageTypes array and adding a Set By Caller Magnitude to an Effect Spec for each damage type.
 *
 * \note This class is implemented as a blueprint callable function.
 */
UCLASS()
class ARIA_API UAriaDamageGameplayAbility : public UAriaGameplayAbility
{
	GENERATED_BODY()

public:
	// This function is used for looping through all of the Damage Types in the DamageTypes array, and for each one, adding a Set By Caller Magnitude to an Effect Spec
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	// Helper function for retrieving the Damage Amount at the current Ability Level
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
	
    UFUNCTION(BlueprintPure)
	FAbilityModifier MakeAbilityModifierFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure)
	FAbilityModifier MakeFinalAbilityModifierFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Ability Modifiers
#pragma region AbilityModifiers
	// The Type of Damage to apply
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Modifiers | Active")
	FGameplayTag DamageType;

	// The Amount of Damage caused by the Damage Type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Modifiers | Active")
	FScalableFloat Damage;

	// Apply Splash Damage
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Active")
	FSplashDamage SplashDamage;
	
	// The chance % that a Knockback will occur
	//  Setting the value at 100 will always cause the Target to be knocked back)
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Active",
		meta = (DisplayName = "Knockback Chance %", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"
		))
	float KnockbackChance = 0.f;

	// This will cause the Target to be knocked backwards when hit
	//    Note: The Target must have a Movement Component set up for this to work
	//  The amount (distance) that they are knocked backwards is based on the value
	//   (Higher values mean farther distance)
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Active")
	float KnockbackForceMagnitude = 0.f;

	// This will cause the Target to be flung back when defeated
	//    Note: The Target must have a Movement Component set up for this to work
	//  The amount (distance) that they fly backwards is based on the value
	//   (Higher values mean farther distance)
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Active")
	float DeathImpulseMagnitude = 0.f;

	// Applies additional Ability Modifiers that affect a Target's Status
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Active")
	FStatusEffect StatusEffect;
#pragma endregion AbilityModifiers

	// End Ability Modifiers
	//  These are mainly used for Abilities that cause Damage over Time and will apply
	//  a Final set of unique Modifiers to the Ability once ended
	//   (Ex: Applying a Knockback or Status Effect once the Ability Ends)
#pragma region EndAbilityModifiers
	// The Damage Type to apply at the end of the Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Modifiers | Final")
	FGameplayTag FinalDamageType;

	// The Amount of Damage caused by the Final Damage Type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Modifiers | Final")
	FScalableFloat FinalDamage;

	// Apply Splash Damage at the end of the Ability
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Final")
	FSplashDamage FinalSplashDamage;

	// The chance % that a Knockback will occur at the end of the Ability
	//  Setting the value at 100 will always cause the Target to be knocked back)
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Final",
		meta = (DisplayName = "Final Knockback Chance %", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"
		))
	float FinalKnockbackChance = 0.f;
	
	// This will cause the Target to be knocked backwards at the end of the Ability
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Final")
	float FinalKnockbackForceMagnitude = 0.f;

	// This will cause the Target to be flung back when defeated on the end of the Ability
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Final")
	float FinalDeathImpulseMagnitude = 0.f;

	// Applies an Additional Status Effect at the end of the Ability
	UPROPERTY(EditDefaultsOnly, Category = "Ability Modifiers | Final")
	FStatusEffect FinalStatusEffect;
#pragma endregion
	
	// The primary Damage Attack Modifier that gets applied to the Gameplay Ability
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	// FAbilityModifier AbilityModifier;

	// The additional Damage Attack Modifiers that get applied to the Gameplay Ability
	// UPROPERTY(EditDefaultsOnly, Category = "Damage")
	// TArray<FAbilityModifier> MultiAbilityModifiers;

	// This function pulls from the list of Attack Montages and returns a random Montage to play
	UFUNCTION(BlueprintPure)
	static FTaggedAttackMontage GetRandomAttackMontage(const TArray<FTaggedAttackMontage>& TaggedAttackMontages);
};
