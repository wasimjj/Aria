// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AriaAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Containers/UnrealString.h"
#include "Templates/SharedPointer.h"
#include "AriaStatusEffectContext.generated.h"

/**
 * Structure representing the status effect context of a gameplay effect in the AriaAbilitySystem.
 *
 * This struct extends the FGameplayEffectContext class and adds additional properties
 * and methods specific to the AriaAbilitySystem.
 */
USTRUCT(BlueprintType)
struct FAriaStatusEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    // Getters for the Status Effect Context
	/* Status Effect Core Properties */
    bool IsStatusEffectApplied() const { return bIsStatusEffectApplied; }
    TSharedPtr<FGameplayTag> GetStatusEffectType() const { return StatusEffectType; }
    float GetStatusEffectDuration() const { return StatusEffectDuration; }
    float GetStatusEffectFrequency() const { return StatusEffectFrequency; }
	/* Status Effect Damage */
    //TSharedPtr<FGameplayTag> GetDamagedAttributeType() const { return DamagedAttributeType; }
	TSharedPtr<FGameplayTag> GetDamagedAttributeType() const 
    {
    	if (!DamagedAttributeType.IsValid())
    		UE_LOG(LogTemp, Warning, TEXT("DamagedAttributeType is invalid or null while getting"));

    	return DamagedAttributeType;
    }
    float GetDamagedAttributeAmount() const { return DamagedAttributeAmount; }
    EAdditionalDamageTriggers GetAdditionalDamageTrigger() const { return AdditionalDamageTrigger; }
    float GetAdditionalDamageAmount() const { return AdditionalDamageAmount; }
	/* Action Hindering */
    ERandomlyMissAnyActions GetRandomlyMissActions() const { return RandomlyMissActions; }
    float GetRandomMissPercentage() const { return RandomMissPercentage; }
    ERandomlyStopAnyActions GetRandomlyStopActions() const { return RandomlyStopActions; }
    float GetReducedActionPercentage() const { return ReducedActionPercentage; }
    EStopAllActions GetStopAllActions() const { return StopAllActions; }
	/* Movement Hindering */
    float GetRReducedMovementSpeed() const { return ReducedMovementSpeed; }
    bool IsRandomizeMovements() const { return bRandomizeMovements; }
    bool IsRunFromTarget() const { return bRunFromTarget; }
	/* Other */
    bool IsInstantKillTarget() const { return bInstantKillTarget; }
    int32 GetReducedLevels() const { return ReducedLevels; }
    bool IsForceAIControl() const { return bForceAIControl; }
	
    // Setters for the Status Effect Context
	/* Status Effect Core Properties */
	void SetIsStatusEffectApplied(const bool bInIsStatusEffectApplied) { bIsStatusEffectApplied = bInIsStatusEffectApplied; }
	void SetStatusEffectType(const TSharedPtr<FGameplayTag>& InStatusEffectType) { StatusEffectType = InStatusEffectType; }
	void SetStatusEffectDuration(const float InStatusEffectDuration) { StatusEffectDuration = InStatusEffectDuration; }
	void SetStatusEffectFrequency(const float InStatusEffectFrequency) { StatusEffectFrequency = InStatusEffectFrequency; }
	/* Status Effect Damage */
	//void SetDamagedAttributeType(const TSharedPtr<FGameplayTag>& InDamagedAttributeType) { DamagedAttributeType = InDamagedAttributeType; }
	void SetDamagedAttributeType(const TSharedPtr<FGameplayTag>& InDamagedAttributeType)
    {
    	if (!InDamagedAttributeType.IsValid())
    	{
    		UE_LOG(LogTemp, Warning, TEXT("InDamagedAttributeType is invalid or null."));
    		return;
    	}

    	if (InDamagedAttributeType == nullptr)
    	{
    		UE_LOG(LogTemp, Warning, TEXT("InDamagedAttributeType is a null pointer."));
    		return;
    	}

    	UE_LOG(LogTemp, Log, TEXT("Setting DamagedAttributeType..."));
    	DamagedAttributeType = InDamagedAttributeType;
    
    	if (DamagedAttributeType.IsValid())
    	{
    		UE_LOG(LogTemp, Log, TEXT("DamagedAttributeType set successfully."));
    	}
    	else
    	{
    		UE_LOG(LogTemp, Warning, TEXT("Failed to set DamagedAttributeType."));
    	}
    }
	void SetDamagedAttributeAmount(const float InDamagedAttributeAmount) { DamagedAttributeAmount = InDamagedAttributeAmount; }
	void SetAdditionalDamageTrigger(const EAdditionalDamageTriggers InAdditionalDamageTrigger) { AdditionalDamageTrigger = InAdditionalDamageTrigger; }
	void SetAdditionalDamageAmount(const float InAdditionalDamageAmount) { AdditionalDamageAmount = InAdditionalDamageAmount; }
	/* Action Hindering */
	void SetRandomlyMissActions(const ERandomlyMissAnyActions InRandomlyMissActions) { RandomlyMissActions = InRandomlyMissActions; }
	void SetRandomMissPercentage(const float InRandomMissPercentage) { RandomMissPercentage = InRandomMissPercentage; }
	void SetRandomlyStopActions(const ERandomlyStopAnyActions InRandomlyStopActions) { RandomlyStopActions = InRandomlyStopActions; }
	void SetReducedActionPercentage(const float InReducedActionPercentage) { ReducedActionPercentage = InReducedActionPercentage; }
	void SetStopAllActions(const EStopAllActions InStopAllActions) { StopAllActions = InStopAllActions; }
	/* Movement Hindering */
	void SetReducedMovementSpeed(const float InReducedMovementSpeed) { ReducedMovementSpeed = InReducedMovementSpeed; }
	void SetIsRandomizeMovements(const bool InIsRandomizeMovements) { bRandomizeMovements = InIsRandomizeMovements; }
	void SetIsRunFromTarget(const bool InIsRunFromTarget) { bRunFromTarget = InIsRunFromTarget; }
	/* Other */
	void SetIsInstantKillTarget(const bool InIsInstantKillTarget) { bInstantKillTarget = InIsInstantKillTarget; }
	void SetReducedLevels(const float InReducedLevels) { ReducedLevels = InReducedLevels; }
	void SetIsIForceAIControl(const bool InIsForceAIControl) { bForceAIControl = InIsForceAIControl; }

	bool bIsAriaStatusEffectContext = true;
	
    /** Returns the original struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return StaticStruct();
    }
    
    /** Creates a copy of this context, used to duplicate for later modifications */
    virtual FAriaStatusEffectContext* Duplicate() const override
    {
        FAriaStatusEffectContext* NewContext = new FAriaStatusEffectContext();
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
	/* Status Effect Core Properties */
	UPROPERTY()
	bool bIsStatusEffectApplied = false;

	TSharedPtr<FGameplayTag> StatusEffectType;

	UPROPERTY()
	float StatusEffectDuration = 0.f;

	UPROPERTY()
	float StatusEffectFrequency = 0.f;

	/* Status Effect Damage */
	TSharedPtr<FGameplayTag> DamagedAttributeType;

	UPROPERTY()
	float DamagedAttributeAmount = 0.f;

	UPROPERTY()
	EAdditionalDamageTriggers AdditionalDamageTrigger = EAdditionalDamageTriggers::Disabled;

	UPROPERTY()
	float AdditionalDamageAmount = 0.f;

	/* Action Hindering */
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

	/* Movement Hindering */
	UPROPERTY()
	float ReducedMovementSpeed = 0.f;

	UPROPERTY()
	bool bRandomizeMovements = false;

	UPROPERTY()
	bool bRunFromTarget = false;

	/* Other */
	UPROPERTY()
	bool bInstantKillTarget = false;

	UPROPERTY()
	int32 ReducedLevels = 0;

	UPROPERTY()
	bool bForceAIControl = false;
};

template <>
struct TStructOpsTypeTraits<FAriaStatusEffectContext> : TStructOpsTypeTraitsBase2<FAriaStatusEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};
