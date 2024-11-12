// Copyright Savoie Interactive Studios


#include "UI/WidgetController/AbilityMenuWidgetController.h"

#include "AriaGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"

#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "Player/AriaPlayerState.h"

void UAbilityMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	// Retrieve the Ability Points total
	AbilityPointsChangedDelegate.Broadcast(GetAriaPlayerState()->GetAbilityPoints());
}

void UAbilityMenuWidgetController::BindCallbacksToDependencies()
{
	GetAriaPlayerState()->OnAbilityPointsChangedDelegate.AddLambda([this](int32 AbilityPoints)
	{
		AbilityPointsChangedDelegate.Broadcast(AbilityPoints);

		// Everytime the Ability Points changes; need to Update the Current Ability Points and Broadcast it
		//  Note: This fixes the issue where the Ability Menu buttons wouldn't update after leveling up while the menu was opened
		CurrentAbilityPoints = AbilityPoints;

		bool bEnableSpendPointsBtn = false;
		bool bEnableEquipBtn = false;
		ShouldEnableAbilityMenuButtons(SelectedAbility.Status, CurrentAbilityPoints, bEnableSpendPointsBtn,
		                               bEnableEquipBtn);
		FText Description;
		FText NextLevelDesc;
		GetAriaAsc()->GetAbilityDescriptionsFromAbilityTag(SelectedAbility.Ability, Description, NextLevelDesc);

		AbilityGlobeSelectedDelegate.Broadcast(bEnableSpendPointsBtn, bEnableEquipBtn, Description, NextLevelDesc);
	});

	GetAriaAsc()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, int32 NewAbilityLevel)
		{
			// Everytime the Ability Status changes; need to Update the Current Ability Status and Broadcast it
			//  Note: This fixes the issue where the Ability Menu buttons wouldn't update after leveling up while the menu was opened 
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = AbilityStatusTag;

				bool bEnableSpendPointsBtn = false;
				bool bEnableEquipBtn = false;
				ShouldEnableAbilityMenuButtons(AbilityStatusTag, CurrentAbilityPoints, bEnableSpendPointsBtn,
				                               bEnableEquipBtn);
				FText Description;
				FText NextLevelDesc;
				GetAriaAsc()->GetAbilityDescriptionsFromAbilityTag(AbilityTag, Description, NextLevelDesc);

				AbilityGlobeSelectedDelegate.Broadcast(bEnableSpendPointsBtn, bEnableEquipBtn, Description,
				                                       NextLevelDesc);
			}

			// Broadcast the Ability Info whenever the Ability Status changes
			if (AbilityInfo)
			{
				FAriaAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.AbilityStatus = AbilityStatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAriaAsc()->AbilityEquippedDelegate.AddUObject(this, &UAbilityMenuWidgetController::OnAbilityEquipped);
}

void UAbilityMenuWidgetController::SpendPointsButtonPressed()
{
	if (GetAriaAsc())
	{
		GetAriaAsc()->ServerAbilityPointsSpent(SelectedAbility.Ability);
	}
}

void UAbilityMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	// Check if the Ability is currently equipped;
	const FGameplayTag SelectedAbilityStatus = GetAriaAsc()->GetAbilityStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedAbilityStatus.MatchesTagExact(FAriaGameplayTags::Get().Abilities_Status_Equipped))
	{
		//  Currently Equipped so Save off that Ability Slot (i.e. Input Activation Tag); since we may need to clear/move it later
		SelectedAbilitySlot = GetAriaAsc()->GetInputActivationTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void UAbilityMenuWidgetController::EquippedAbilityGlobePressed(const FGameplayTag& EquipSlotAbilityTag,
                                                               const FGameplayTag& EquipSlotAbilityType)
{
	if (!bWaitingForEquipSelection) return;

	// One of the Equip Ability Row Globes has been selected...

	// Check Selected Ability Type against the Selected Globe's Ability Type to make sure they match
	//  (prevents equipping Abilities in slots of the wrong ability type)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(EquipSlotAbilityType)) return;

	// Try to equip the Selected Ability into the Ability Equip Slot
	//  (i.e. Setting the Abilities Input Activation Tag to the Input Tag of the Selected Slot)
	GetAriaAsc()->ServerEquipAbility(SelectedAbility.Ability, EquipSlotAbilityTag);
}

void UAbilityMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus,
                                                     const FGameplayTag& NewSlot, const FGameplayTag& OldSlot)
{
	bWaitingForEquipSelection = false;

	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	// Clear out the old Slot data
	// Set the Previous Slot Info to Empty
	FAriaAbilityInfo PreviousSlotInfo;
	PreviousSlotInfo.AbilityStatus = GameplayTags.Abilities_Status_Unlocked;
	PreviousSlotInfo.InputActivationTag = OldSlot;
	PreviousSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// Broadcast Empty Info for the Previous Slot; if its a valid Slot
	//  This also helps the Equipped Ability Row ignore any need to perform changes
	//   if attempting to equip an already-equipped ability
	AbilityInfoDelegate.Broadcast(PreviousSlotInfo);

	// Fill in the new Slot data
	// Get the Ability Info for the new Slot
	FAriaAbilityInfo NewSlotInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	NewSlotInfo.AbilityStatus = AbilityStatus;
	NewSlotInfo.InputActivationTag = NewSlot;

	// Broadcast the Info for the New Slot
	AbilityInfoDelegate.Broadcast(NewSlotInfo);

	// Force a 'cancel waiting for Equip Selection'
	CancelWaitForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);

	// Clear out the Descriptions Boxes, reset the Buttons, and Deselect the Ability Tree Globe
	AbilityGlobeReassignedDelegate.Broadcast(AbilityTag);
	AbilityGlobeDeselect();
}

void UAbilityMenuWidgetController::AbilityGlobeSelected(const FGameplayTag& AbilityTag)
{
	// If currently waiting for an Ability Equip; then cancel waiting if that ability is deselected.
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		CancelWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	// Select the Ability Globe
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();
	const FGameplayAbilitySpec* AbilitySpec = GetAriaAsc()->GetSpecFromAbilityTag(AbilityTag);
	const int32 AbilityPoints = GetAriaPlayerState()->GetAbilityPoints();
	FGameplayTag AbilityStatus;

	// Determine the Ability Status for the provided Ability Tag
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		// Either dealing with an Ability Tag for an Ability that we don't have yet
		// or an Ability Tag for an Ability that doesn't exist (ex; Abilities_None)
		// Treat this as a Locked Ability
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAriaAsc()->GetAbilityStatusTagFromSpec(*AbilitySpec);
	}

	// Stores off the Selected Ability details for the Ability currently selected in the Ability Menu
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	// Check whether or not to enable those buttons
	bool bEnableSpendPointsBtn = false;
	bool bEnableEquipBtn = false;
	ShouldEnableAbilityMenuButtons(AbilityStatus, AbilityPoints, bEnableSpendPointsBtn, bEnableEquipBtn);
	FText Description;
	FText NextLevelDesc;
	GetAriaAsc()->GetAbilityDescriptionsFromAbilityTag(AbilityTag, Description, NextLevelDesc);

	AbilityGlobeSelectedDelegate.Broadcast(bEnableSpendPointsBtn, bEnableEquipBtn, Description, NextLevelDesc);
}

void UAbilityMenuWidgetController::AbilityGlobeDeselect()
{
	// If currently waiting for an Ability Equip; then cancel waiting if that ability is deselected.
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).
		                                                      AbilityType;
		CancelWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	// Deselect the active Ability Globe
	SelectedAbility.Ability = FAriaGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAriaGameplayTags::Get().Abilities_Status_Locked;
	AbilityGlobeSelectedDelegate.Broadcast(false, false, FText(), FText());
}

void UAbilityMenuWidgetController::ShouldEnableAbilityMenuButtons(const FGameplayTag AbilityStatus,
                                                                  const int32 AbilityPoints,
                                                                  bool& bShouldEnableAbilityPointsBtn,
                                                                  bool& bShouldEnableEquipBtn)
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableEquipBtn = false;
		bShouldEnableAbilityPointsBtn = false;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquipBtn = false;
		bShouldEnableAbilityPointsBtn = AbilityPoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipBtn = true;
		bShouldEnableAbilityPointsBtn = AbilityPoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipBtn = true;
		bShouldEnableAbilityPointsBtn = AbilityPoints > 0;
	}
}
