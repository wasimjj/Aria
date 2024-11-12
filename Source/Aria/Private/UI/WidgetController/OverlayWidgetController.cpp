// Copyright Savoie Interactive Studios


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AriaGameplayTags.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AriaPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// As soon as BroadcastInitialValues is called, we'll broadcast the values of the Actual Attributes in the AttributeSet.

	// Used for updating the Health and Mana Progress Widgets (ex: Globe Health & Mana)
	OnHealthChanged.Broadcast(GetAriaAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAriaAttributeSet()->GetMaxHealth());

	OnManaChanged.Broadcast(GetAriaAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAriaAttributeSet()->GetMaxMana());

	// Forces the XP Bar to refresh when first loading the Hud (Overlay)
	OnXPChanged(0);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	/* AriaPlayerState Callbacks */
#pragma region AriaPlayerState_Callbacks
	GetAriaPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAriaPlayerState()->OnLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
#pragma endregion

	/* AriaAbilitySystem Callbacks */
#pragma region AriaAbilitySystem_Callbacks
	if (GetAriaAsc())
	{
		GetAriaAsc()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
	}
#pragma endregion
	
	/* AriaAttributeSet Callbacks */
#pragma region AriaAttributeSet_Callbacks
	// We need a callback that we can bind to that Delegate that is Broadcast whenever Attributes change.
	// This whole expression returns that Delegate
	// So whenever an Attribute changes, the associated function callback will be called.
	// UPDATE: Converted to Lambdas and removed the Callback functions.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAriaAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAriaAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAriaAttributeSet()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAriaAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (GetAriaAsc())
	{
		if (GetAriaAsc()->bStartupAbilitiesGiven)
		{
			// If AbilitiesGiven then directly call the OnInitializeStartupAbilities function
			BroadcastAbilityInfo();
		}
		else
		{
			// If AbilitiesGiven haven't been given (Broadcast hasn't been made yet) then binds the AbilitiesGivenDelegate to the OnInitializeStartupAbilities function
			GetAriaAsc()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		// Binds the EffectAssetTags delegate using a Lambda
		GetAriaAsc()->EffectAssetTagsDelegate.AddLambda(
			// The signature for a Lambda: [](){}
			// The [] are used for Captures (Variables can be captures, also capturing by reference or by pointer)
			//   Important: If you want to call a member function in a Lambda, you have to capture
			//   that Object of the Class that that function belongs to. 
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

					/* Tag match Example:
					 *   Tag = Message.HealthPotion
					 *   Formula for matching:
					 *      "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
					 *   "Message.HealthPotion".MatchesTag("Message") will return True. "Message".MatchesTag("Message.HealthPotion") will return False.
					*/

					// Only look for the Row and Broadcast it if the Tag type is a Message Tag.
					// If its any other Tag (ex: Ability Tag. Input Tag, etc), we don't want to look for a Widget Row in our Message Widget Data Table
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = UAriaAbilitySystemLibrary::GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
#pragma endregion
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXPAmount)
{
	const ULevelUpInfo* LevelUpInfo = GetAriaPlayerState()->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please set in BP_AriaPlayerState Blueprint"));

	// Determines what the XP Bar Percentage should be at based on the current XP amount
	const int32 Level = LevelUpInfo->FindLevelBasedOnTotalXP(NewXPAmount);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		// Stores the XP needed to reach the next level and also the XP Base Amount (retrieved from the Previous Level)
		//  Note: This assists the XP Bar fill percentage needed
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		// Gets the Delta Level Requirement (The Current Next Level Up Requirement - Previous Level Up Requirement)
		//  Note: This is needed; since we must divide our current XP by this value
		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;

		const int32 XPForThisLevel = NewXPAmount - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);

		OnXPChangedDelegate.Broadcast(XPForThisLevel, LevelUpRequirement, XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus,
                                                 const FGameplayTag& NewSlot, const FGameplayTag& OldSlot) const
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	// Clear out the old Slot data
	// Set the Previous Slot Info to Empty
	FAriaAbilityInfo PreviousSlotInfo;
	PreviousSlotInfo.AbilityStatus = GameplayTags.Abilities_Status_Unlocked;
	PreviousSlotInfo.InputActivationTag = OldSlot;
	PreviousSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// Broadcast Empty Info for the Previous Slot;
	AbilityInfoDelegate.Broadcast(PreviousSlotInfo);

	// Fill in the new Slot data
	// Get the Ability Info for the new Slot
	FAriaAbilityInfo NewSlotInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	NewSlotInfo.AbilityStatus = AbilityStatus;
	NewSlotInfo.InputActivationTag = NewSlot;

	// Broadcast the Info for the New Slot
	AbilityInfoDelegate.Broadcast(NewSlotInfo);
}
