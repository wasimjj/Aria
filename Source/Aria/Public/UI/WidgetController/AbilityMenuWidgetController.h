// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AriaGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AriaWidgetController.h"
#include "AbilityMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAbilityGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool,
                                              bEquipButtonEnabled, FText, AbilityDescription, FText,
                                              AbilityNextLevelDesc);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

// Only used as an internal data structure to help keep track of the Status of an Ability
struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class ARIA_API UAbilityMenuWidgetController : public UAriaWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature AbilityPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAbilityGlobeSelectedSignature AbilityGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAbilityGlobeReassignedSignature AbilityGlobeReassignedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature CancelWaitForEquipDelegate;


	UFUNCTION(BlueprintCallable)
	void SpendPointsButtonPressed();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void EquippedAbilityGlobePressed(const FGameplayTag& EquipSlotAbilityTag, const FGameplayTag& EquipSlotAbilityType);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag,
	                       const FGameplayTag& AbilityStatus, const FGameplayTag& NewSlot, const FGameplayTag& OldSlot);

	UFUNCTION(BlueprintCallable)
	void AbilityGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void AbilityGlobeDeselect();

private:
	// Determines whether the Spend Points and Equip menu buttons should be enabled
	static void ShouldEnableAbilityMenuButtons(const FGameplayTag AbilityStatus, const int32 AbilityPoints,
	                                           bool& bShouldEnableAbilityPointsBtn, bool& bShouldEnableEquipBtn);

	FSelectedAbility SelectedAbility = {
		FAriaGameplayTags::Get().Abilities_None, FAriaGameplayTags::Get().Abilities_Status_Locked
	};
	int32 CurrentAbilityPoints = 0;

	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectedAbilitySlot;
};
