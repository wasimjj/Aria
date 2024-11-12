// Copyright Savoie Interactive Studios

#pragma once

#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "AriaAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAdditionalDamageTriggers : uint8
{
	Disabled UMETA(DisplayName = "Disabled"),
	Actions_Attacks UMETA(DisplayName = "Actions (Attacks)"),
	Actions_Abilities UMETA(DisplayName = "Actions (Abilities)"),
	Actions_Both UMETA(DisplayName = "Actions (Both)"),
	Movement UMETA(DisplayName = "Movement"),
	All UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class ERandomlyMissAnyActions : uint8
{
	Disabled UMETA(DisplayName = "Disabled"),
	Attacks UMETA(DisplayName = "Attacks"),
	Abilities UMETA(DisplayName = "Abilities"),
	Both UMETA(DisplayName = "Both"),
};

UENUM(BlueprintType)
enum class ERandomlyStopAnyActions : uint8
{
	Disabled UMETA(DisplayName = "Disabled"),
	Attacks UMETA(DisplayName = "Attacks"),
	Abilities UMETA(DisplayName = "Abilities"),
	Both UMETA(DisplayName = "Both"),
};

UENUM(BlueprintType)
enum class EStopAllActions : uint8
{
	Disabled UMETA(DisplayName = "Disabled"),
	Attacks UMETA(DisplayName = "Attacks"),
	Abilities UMETA(DisplayName = "Abilities"),
	Both UMETA(DisplayName = "Both"),
};

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAbilityModifier
{
	GENERATED_BODY()

	FAbilityModifier()
	{
	}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAsc;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAsc;

	UPROPERTY(BlueprintReadWrite)
	int32 AbilityLevel = 1.f;

	/* Damage */

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	/* Knockbacks */

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	/* Status Effects */

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag StatusEffectType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float StatusEffectChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float StatusEffectDurationMin = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float StatusEffectDurationMax = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float StatusEffectFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamagedAttributeType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DamagedAttributeAmount = 0.f;

	UPROPERTY(BlueprintReadWrite)
	EAdditionalDamageTriggers AdditionalDamageTrigger = EAdditionalDamageTriggers::Disabled;

	UPROPERTY(BlueprintReadWrite)
	float AdditionalDamageAmount = 0.f;

	UPROPERTY(BlueprintReadWrite)
	ERandomlyMissAnyActions RandomlyMissActions = ERandomlyMissAnyActions::Disabled;

	UPROPERTY(BlueprintReadWrite)
	float RandomMissPercentage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	ERandomlyStopAnyActions RandomlyStopActions = ERandomlyStopAnyActions::Disabled;

	UPROPERTY(BlueprintReadWrite)
	float ReducedActionPercentage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	EStopAllActions StopAllActions = EStopAllActions::Disabled;

	UPROPERTY(BlueprintReadWrite)
	float ReducedMovementSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bRandomizeMovements = false;

	UPROPERTY(BlueprintReadWrite)
	bool bRunFromTarget = false;

	UPROPERTY(BlueprintReadWrite)
	bool bInstantKillTarget = false;

	UPROPERTY(BlueprintReadWrite)
	int32 ReducedLevels = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bForceAIControl = false;
};

/**
 * Structure representing the context of a gameplay effect in the AriaAbilitySystem.
 *
 * This struct extends the FGameplayEffectContext class and adds additional properties
 * and methods specific to the AriaAbilitySystem.
 */
USTRUCT(BlueprintType)
struct FAriaGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	// Getters for the Effect Context
	bool IsDodgedHit() const { return bIsDodgedHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	bool IsStatusEffectApplied() const { return bIsStatusEffectApplied; }
	TSharedPtr<FGameplayTag> GetStatusEffectType() const { return StatusEffectType; }
	float GetStatusEffectDuration() const { return StatusEffectDuration; }
	float GetStatusEffectFrequency() const { return StatusEffectFrequency; }
	TSharedPtr<FGameplayTag> GetDamagedAttributeType() const { return DamagedAttributeType; }
	float GetDamagedAttributeAmount() const { return DamagedAttributeAmount; }
	EAdditionalDamageTriggers GetAdditionalDamageTrigger() const { return AdditionalDamageTrigger; }
	float GetAdditionalDamageAmount() const { return AdditionalDamageAmount; }
	ERandomlyMissAnyActions GetRandomlyMissActions() const { return RandomlyMissActions; }
	float GetRandomMissPercentage() const { return RandomMissPercentage; }
	ERandomlyStopAnyActions GetRandomlyStopActions() const { return RandomlyStopActions; }
	float GetReducedActionPercentage() const { return ReducedActionPercentage; }
	EStopAllActions GetStopAllActions() const { return StopAllActions; }
	float GetRReducedMovementSpeed() const { return ReducedMovementSpeed; }
	bool IsRandomizeMovements() const { return bRandomizeMovements; }
	bool IsRunFromTarget() const { return bRunFromTarget; }
	bool IsInstantKillTarget() const { return bInstantKillTarget; }
	int32 GetReducedLevels() const { return ReducedLevels; }
	bool IsForceAIControl() const { return bForceAIControl; }

	// Setters for the Effect Context
	void SetIsDodgedHit(const bool bInIsDodgedHit) { bIsDodgedHit = bInIsDodgedHit; }
	void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }

	void SetIsStatusEffectApplied(const bool bInIsStatusEffectApplied)
	{
		bIsStatusEffectApplied = bInIsStatusEffectApplied;
	}

	void SetStatusEffectType(const TSharedPtr<FGameplayTag>& InStatusEffectType)
	{
		StatusEffectType = InStatusEffectType;
	}

	void SetStatusEffectDuration(const float InStatusEffectDuration) { StatusEffectDuration = InStatusEffectDuration; }

	void SetStatusEffectFrequency(const float InStatusEffectFrequency)
	{
		StatusEffectFrequency = InStatusEffectFrequency;
	}

	void SetDamagedAttributeType(const TSharedPtr<FGameplayTag>& InDamagedAttributeType)
	{
		DamagedAttributeType = InDamagedAttributeType;
	}

	void SetDamagedAttributeAmount(const float InDamagedAttributeAmount)
	{
		DamagedAttributeAmount = InDamagedAttributeAmount;
	}

	void SetAdditionalDamageTrigger(const EAdditionalDamageTriggers InAdditionalDamageTrigger)
	{
		AdditionalDamageTrigger = InAdditionalDamageTrigger;
	}

	void SetAdditionalDamageAmount(const float InAdditionalDamageAmount)
	{
		AdditionalDamageAmount = InAdditionalDamageAmount;
	}

	void SetRandomlyMissActions(const ERandomlyMissAnyActions InRandomlyMissActions)
	{
		RandomlyMissActions = InRandomlyMissActions;
	}

	void SetRandomMissPercentage(const float InRandomMissPercentage) { RandomMissPercentage = InRandomMissPercentage; }

	void SetRandomlyStopActions(const ERandomlyStopAnyActions InRandomlyStopActions)
	{
		RandomlyStopActions = InRandomlyStopActions;
	}

	void SetReducedActionPercentage(const float InReducedActionPercentage)
	{
		ReducedActionPercentage = InReducedActionPercentage;
	}

	void SetStopAllActions(const EStopAllActions InStopAllActions) { StopAllActions = InStopAllActions; }
	void SetReducedMovementSpeed(const float InReducedMovementSpeed) { ReducedMovementSpeed = InReducedMovementSpeed; }
	void SetIsRandomizeMovements(const bool InIsRandomizeMovements) { bRandomizeMovements = InIsRandomizeMovements; }
	void SetIsRunFromTarget(const bool InIsRunFromTarget) { bRunFromTarget = InIsRunFromTarget; }
	void SetIsInstantKillTarget(const bool InIsInstantKillTarget) { bInstantKillTarget = InIsInstantKillTarget; }
	void SetReducedLevels(const float InReducedLevels) { ReducedLevels = InReducedLevels; }
	void SetIsIForceAIControl(const bool InIsForceAIControl) { bForceAIControl = InIsForceAIControl; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAriaGameplayEffectContext* Duplicate() const override
	{
		FAriaGameplayEffectContext* NewContext = new FAriaGameplayEffectContext();
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
	UPROPERTY()
	bool bIsDodgedHit = false;

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	/* Damage */

	// Does not need a UPROPERTY(); cleanup is already handled by the memory manager
	TSharedPtr<FGameplayTag> DamageType;

	/* Knockbacks */

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	/* Status Effects */

	UPROPERTY()
	bool bIsStatusEffectApplied = false;

	TSharedPtr<FGameplayTag> StatusEffectType;

	UPROPERTY()
	float StatusEffectDuration = 0.f;

	UPROPERTY()
	float StatusEffectFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamagedAttributeType;

	UPROPERTY()
	float DamagedAttributeAmount = 0.f;

	UPROPERTY()
	EAdditionalDamageTriggers AdditionalDamageTrigger = EAdditionalDamageTriggers::Disabled;

	UPROPERTY()
	float AdditionalDamageAmount = 0.f;

	UPROPERTY()
	ERandomlyMissAnyActions RandomlyMissActions = ERandomlyMissAnyActions::Disabled;

	UPROPERTY()
	float RandomMissPercentage = 0.f;

	UPROPERTY()
	ERandomlyStopAnyActions RandomlyStopActions = ERandomlyStopAnyActions::Disabled;

	UPROPERTY()
	float ReducedActionPercentage = 0.f;

	UPROPERTY()
	EStopAllActions StopAllActions = EStopAllActions::Disabled;

	UPROPERTY()
	float ReducedMovementSpeed = 0.f;

	UPROPERTY()
	bool bRandomizeMovements = false;

	UPROPERTY()
	bool bRunFromTarget = false;

	UPROPERTY()
	bool bInstantKillTarget = false;

	UPROPERTY()
	int32 ReducedLevels = 0;

	UPROPERTY()
	bool bForceAIControl = false;
};

template <>
struct TStructOpsTypeTraits<FAriaGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAriaGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
