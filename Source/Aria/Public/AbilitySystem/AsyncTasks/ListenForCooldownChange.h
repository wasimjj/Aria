// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "ListenForCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

// Delegate used for broadcasting the cooldown duration value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * \class UListenForCooldownChange
 *
 * \brief A BlueprintAsyncActionBase class that allows for listening to cooldown changes in an AbilitySystemComponent.
 *
 * The UListenForCooldownChange class provides a way to listen for changes in cooldowns applied to an AbilitySystemComponent (ASC) and execute actions when a cooldown starts or ends. It is primarily used in Ability-based Widgets like the Ability Globe, which needs to know the type of cooldown tag it is listening for.
 *
 * Usage:
 *
 * 1. Call the static method WaitForCooldownChange to create a new UListenForCooldownChange object and start listening for cooldown changes.
 *
 * 2. Specify the AbilitySystemComponent and the cooldown tag to listen for.
 *
 * 3. Optionally, bind to the CooldownStart and CooldownEnd events to execute actions when a cooldown starts or ends.
 *
 * 4. Call the EndTask method to stop listening for cooldown changes and clean up resources.
 *
 * Example:
 * \code
 * UListenForCooldownChange* CooldownListener = UListenForCooldownChange::WaitForCooldownChange(AbilitySystemComponent, CooldownTag);
 * if (CooldownListener)
 * {
 *     CooldownListener->CooldownStart.AddUObject(this, &MyClass::OnCooldownStart);
 *     CooldownListener->CooldownEnd.AddUObject(this, &MyClass::OnCooldownEnd);
 * }
 * \endcode
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class ARIA_API UListenForCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	// The node will have two Execution Pins for the Cooldown Start and End
	//  These will be executed once those Delegates are Broadcast
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	// Needs the ASC so can listen for when something changes on that ASC; specifically when a Cooldown Gameplay Event has been applied
	//  This node gets used in Ability-based Widgets like the Ability Globe; which needs to know what type of Cooldown tag its listening for.
	// Note: BlueprintInternalUseOnly is used when working with ASync Task functions
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UListenForCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
	                                                       const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, const int32 NewCount) const;

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetAsc, const FGameplayEffectSpec& SpecApplied,
	                         FActiveGameplayEffectHandle ActiveEffectHandle) const;
};
