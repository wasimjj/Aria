// Copyright Savoie Interactive Studios


#include "AbilitySystem/AriaAbilitySystemComponent.h"

#include "Game/AriaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AriaGameplayTags.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AriaGameplayAbility.h"
#include "Aria/AriaLogChannels.h"
#include "Interaction/XPInterface.h"

void UAriaAbilitySystemComponent::AbilityActorInfoSet()
{
	// Bind to Delegates

	// When any Effect is applied it will call back to the ClientEffectApplied() function below
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAriaAbilitySystemComponent::ClientEffectApplied);
}

/* Ability-based */
#pragma region Ability Functions
void UAriaAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InitialAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : InitialAbilities)
	{
		// Setting the Ability Level to 1 whenever first added to a Character (Note: This can be changed later if needed) 
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAriaGameplayAbility* AriaAbility = Cast<UAriaGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AriaAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAriaGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;

	// Broadcasts the AbilitiesGiven Delegate
	// Note: The OverlayWidgetController will also be listening for this in order to set the Ability Globes
	AbilitiesGivenDelegate.Broadcast();
}

void UAriaAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& InitialPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> PassiveAbilityClass : InitialPassiveAbilities)
	{
		// Note: Passive Abilities are typically System controlled; so the Ability Level wouldn't really matter in this case; other Passive Abilities may need to be handled differently
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(PassiveAbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAriaAbilitySystemComponent::ForEachAbility(const FForEachAbilitySignature& AbilityDelegate)
{
	// Lock the Activatable Abilities container until the for loop is done
	//  This is due in part to Abilities being able to change their State; which could be bad when still in the middle of the loop
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If the AbilityDelegate has a callback bound to it, then we will attempt to execute that Delegate
		if (!AbilityDelegate.ExecuteIfBound(AbilitySpec))
		{
			// Log the Function name
			UE_LOG(LogAria, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

void UAriaAbilitySystemComponent::AscAbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopedAbilityListLock(*this);
	
	// Check to see if we have any Activatable Abilities associated with this Input Tag
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// This should occur regardless of whether the Ability is active
			AbilitySpecInputPressed(AbilitySpec);

			// Ensure we aren't activating them every single frame while holding down an Input.
			if (AbilitySpec.IsActive())
			{
				// Tells the server that we are releasing the Input
				//  Note: Without this; the WaitInputPressed blueprint node won't function.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UAriaAbilitySystemComponent::AscAbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopedAbilityListLock(*this);
	
	// Check to see if we have any Activatable Abilities associated with this Input Tag
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// This should occur regardless of whether the Ability is active
			AbilitySpecInputPressed(AbilitySpec);

			// Ensure we aren't activating them every single frame while holding down an Input.
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
				break;
			}
		}
	}
}

void UAriaAbilitySystemComponent::AscAbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopedAbilityListLock(*this);
	
	// Check to see if we have any Activatable Abilities associated with this Input Tag
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			
			// Tells the server that we are releasing the Input
			//  Note: Without this; the WaitInputReleased blueprint node won't function.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			break;
		}
	}
}

void UAriaAbilitySystemComponent::UnlockAbilitiesByLevel(const int32 Level)
{
	UAbilityInfo* AbilityInfo = UAriaAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAriaAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		// Check the Abilities Tag
		if (!Info.AbilityTag.IsValid()) continue;
		// Check the Abilities Level Requirement
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			// This Ability meets the Level requirement and is an ability that
			//  the Character (or Object) doesn't have yet
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.GameplayAbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAriaGameplayTags::Get().Abilities_Status_Unlocked);
			GiveAbility(AbilitySpec);

			// Forces Immediate Replication
			MarkAbilitySpecDirty(AbilitySpec);

			// Client RPC Call
			ClientUpdateAbilityStatus(Info.AbilityTag, FAriaGameplayTags::Get().Abilities_Status_Unlocked, 1);
		}
	}
}

void UAriaAbilitySystemComponent::UpdateAbilitiesEligibilityByLevel(const int32 Level)
{
	UAbilityInfo* AbilityInfo = UAriaAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAriaAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		// Check the Abilities Tag
		if (!Info.AbilityTag.IsValid()) continue;
		// Check the Abilities Level Requirement
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			// This Ability meets the Level requirement and is an ability that
			//  the Character (or Object) doesn't have yet
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.GameplayAbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAriaGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);

			// Forces Immediate Replication
			MarkAbilitySpecDirty(AbilitySpec);

			// Client RPC Call
			ClientUpdateAbilityStatus(Info.AbilityTag, FAriaGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

// ** Used with the Ability Equipping System **
#pragma region AbilityEquipSystem
bool UAriaAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopedAbilityListLock(*this);
	
	// Loop through all of our Activatable Abilities and see if an Ability has this Slot
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityBoundToSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UAriaAbilitySystemComponent::AbilityBoundToSlot(const FGameplayAbilitySpec& AbilitySpec,
                                                     const FGameplayTag& Slot)
{
	return AbilitySpec.DynamicAbilityTags.HasTagExact(Slot);
}

bool UAriaAbilitySystemComponent::AbilityBoundToAnySlot(const FGameplayAbilitySpec& AbilitySpec)
{
	// TODO: Change FName to use an FGameplayTag called InputTag
	return AbilitySpec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UAriaAbilitySystemComponent::GetAbilitySpecBoundToSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopedAbilityListLock(*this);
	
	// Loop through all of our Activatable Abilities and return the matching Ability Spec
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAriaAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	const UAbilityInfo* AbilityInfo = UAriaAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const FAriaAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;

	return AbilityType.MatchesTag(FAriaGameplayTags::Get().Abilities_Type_Passive);
}

void UAriaAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& Slot)
{
	// Clear out the old Ability already bound to the Slot 
	ClearBoundAbilityFromSlot(&AbilitySpec);

	// Assign the (Selected) Ability to the new Slot (Input Activation Tag)
	AbilitySpec.DynamicAbilityTags.AddTag(Slot);
}

void UAriaAbilitySystemComponent::ClearBoundAbilityFromSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag SlotInputActivationTag = GetInputActivationTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(SlotInputActivationTag);
}
#pragma endregion

void UAriaAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& PassiveAbilityTag,
	bool bActivate)
{
	ActivatePassiveEffectDelegate.Broadcast(PassiveAbilityTag, bActivate);
}
#pragma endregion

/* Attribute-based Functions */
#pragma region Attribute Functions
void UAriaAbilitySystemComponent::UpdateAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UXPInterface>())
	{
		if (IXPInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpdateAttribute(AttributeTag);
		}
	}
}

void UAriaAbilitySystemComponent::ServerUpdateAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	constexpr float AmountToSpend = 1.f;

	// Increment the Attribute by a preset # of Points (AmountToSpend)
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = AmountToSpend;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	// Deduct the amount (AmountToSpend) of AttributePoints used to increase the Attribute
	if (GetAvatarActor()->Implements<UXPInterface>())
	{
		IXPInterface::Execute_AddToAttributePoints(GetAvatarActor(), -AmountToSpend);
	}
}
#pragma endregion

// ** GetFromAbilityTag Functions
#pragma region GetFromAbilityTag_Functions
FGameplayTag UAriaAbilitySystemComponent::GetAbilityStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetAbilityStatusTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

bool UAriaAbilitySystemComponent::GetAbilityDescriptionsFromAbilityTag(const FGameplayTag& AbilityTag,
                                                                       FText& OutAbilityDescription,
                                                                       FText& OutAbilityNextLevelDesc)
{
	const AAriaGameModeBase* AriaGameMode = Cast<AAriaGameModeBase>(UGameplayStatics::GetGameMode(GetOwner()));
	if (AriaGameMode == nullptr) return false;

	UDataTable* AbilityDescriptionsTable = AriaGameMode->AbilityDescriptionsDataTable;

	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FUIAbilityDescriptionsRow* DescriptionsRow = UAriaAbilitySystemLibrary::GetAbilityDescriptionsRow<
			FUIAbilityDescriptionsRow>(AbilityDescriptionsTable, AbilityTag, AbilitySpec->Level);

		if (DescriptionsRow)
		{
			OutAbilityDescription = DescriptionsRow->AbilityDescription;
			OutAbilityNextLevelDesc = DescriptionsRow->AbilityNextLevelDescription;
			return true;
		}
		else
		{
			// No Descriptions Row was found... Return an empty FText
			OutAbilityDescription = FText();
			return false;
		}
	}

	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAriaGameplayTags::Get().Abilities_None))
	{
		// Handles those special cases where no Ability exists
		OutAbilityDescription = FText();
	}
	else
	{
		// Likely a Locked (or unavailable) Ability
		const FUIAbilityDescriptionsRow* DescriptionsRow = UAriaAbilitySystemLibrary::GetAbilityDescriptionsRow<
			FUIAbilityDescriptionsRow>(AbilityDescriptionsTable, AbilityTag, 1);
		OutAbilityDescription = DescriptionsRow->AbilityLockedDescription;
	}
	OutAbilityNextLevelDesc = FText();
	return false;
}

FGameplayTag UAriaAbilitySystemComponent::GetInputActivationTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputActivationTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}
#pragma endregion

// ** GetFromAbilitySpec Functions **
#pragma region GetFromAbilitySpec_Functions
FGameplayTag UAriaAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag AbilityTag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			// Loop through each Gameplay Tag looking for one that has "Abilities" in the tag name (ex: Abilities.Fire.Firebolt)
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return AbilityTag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UAriaAbilitySystemComponent::GetAbilityTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag AbilityTypeTag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			// Loop through each Gameplay Tag looking for one that has "Abilities.Type" in the tag name (ex: Abilities.Type.Offensive)
			if (AbilityTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Type"))))
			{
				return AbilityTypeTag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UAriaAbilitySystemComponent::GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag AbilityStatusTag : AbilitySpec.DynamicAbilityTags)
	{
		// Loop through each Dynamic Gameplay Tag looking for one that has "InputTag" in the tag name (ex: InputTag_LMB)
		if (AbilityStatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return AbilityStatusTag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAriaAbilitySystemComponent::GetInputActivationTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag InputTag : AbilitySpec.DynamicAbilityTags)
	{
		// Loop through each Dynamic Gameplay Tag looking for one that has "InputTag" in the tag name (ex: InputTag_LMB)
		if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return InputTag;
		}
	}

	return FGameplayTag();
}

FGameplayAbilitySpec* UAriaAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	//  (Locking Scope to avoid complications with Abilities being Added or Removed during iteration)
	FScopedAbilityListLock ActiveScopeLock(*this);

	// Loop through our Activatable Abilities
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}
#pragma endregion

void UAriaAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

/* Public Server RPC Functions */
#pragma region PublicServerRPCs
void UAriaAbilitySystemComponent::ServerAbilityPointsSpent_Implementation(
	const FGameplayTag& AbilityTag, const bool bRestartAbility)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		// Note: If we don't reach here, then this Gameplay Ability doesn't exist in our Activatable Abilities
		//	So it is not a Locked Ability; It is either Unlocked, Eligible, or Equipped

		const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

		if (GetAvatarActor()->Implements<UXPInterface>())
		{
			// First: Check if the Ability can be Unlocked/Leveled Up

			// Get the current Ability Points Available to Spend
			const int32 CurrentAbilityPoints = IXPInterface::Execute_GetAbilityPoints(GetAvatarActor());

			const int32 AbilityLevelUpCost = GetAbilityLevelUpRequirementCost(AbilityTag);
			if (AbilityLevelUpCost > 0 && CurrentAbilityPoints >= AbilityLevelUpCost)
			{
				// Subtract the Ability Cost amount from the total available Ability Points
				IXPInterface::Execute_AddToAbilityPoints(GetAvatarActor(), -AbilityLevelUpCost);

				// Unlock or Level up the Ability
#pragma region UnlockOrLevelUpAbility
				// Get the Status from the Ability Spec
				FGameplayTag AbilityStatus = GetAbilityStatusTagFromSpec(*AbilitySpec);

				if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
				{
					// This Eligible Ability can now be unlocked!
					// First remove the Eligibility Status
					AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
					// Next Add the Unlocked Status
					AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
					// Update Ability Status to the new Status
					AbilityStatus = GameplayTags.Abilities_Status_Unlocked;
				}
				else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) ||
					AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
				{
					// Increments the Ability to the next Level
					//   Note: This does not automatically cancel the Ability 
					AbilitySpec->Level += 1;

					// Stop and Restart the Ability (Optional)
					if (bRestartAbility)
					{
						// First remove the Ability
						ClearAbility(AbilitySpec->Handle);
						// Re-add the Ability
						GiveAbility(*AbilitySpec);
					}
				}

				ClientUpdateAbilityStatus(AbilityTag, AbilityStatus, AbilitySpec->Level);
				MarkAbilitySpecDirty(*AbilitySpec);
#pragma endregion
			}
		}
	}
}

void UAriaAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& SlotToEquip)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

		const FGameplayTag& PrevSlotInputActivationTag = GetInputActivationTagFromSpec(*AbilitySpec);
		const FGameplayTag& AbilityStatus = GetAbilityStatusTagFromSpec(*AbilitySpec);

		// Check if the AbilityStatus for the Ability we want to Equip is either Unlocked or Equipped
		//  If so, we can assign it to the new Slot (Input Activation Tag)
		const bool bStatusValid = AbilityStatus == GameplayTags.Abilities_Status_Unlocked ||
			AbilityStatus == GameplayTags.Abilities_Status_Equipped;
		if (bStatusValid)
		{
			
#pragma region SlotNotEmpty
			if (!SlotIsEmpty(SlotToEquip))
			{
				// There is an Ability in this Slot already; Clear its Slot and push out the Ability
				//  Also if it is a Passive Ability Slot; then Deactivate the Previous Passive Ability

				if (FGameplayAbilitySpec* AbilitySpecBoundToSlot = GetAbilitySpecBoundToSlot(SlotToEquip))
				{
					// Check whether The Ability Slot has the Same Ability that we are trying to Equip; If so, then Exit function and return early
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*AbilitySpecBoundToSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, SlotToEquip, PrevSlotInputActivationTag);
						return;
					}

					// Verify whether this is a Passive Ability
					if (IsPassiveAbility(*AbilitySpecBoundToSlot))
					{
						// Deactivate the Passive Ability effect
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*AbilitySpecBoundToSlot), false);
						
						// Deactivate the Passive Ability for the previous Ability bound to the Slot
						DeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*AbilitySpecBoundToSlot));
					}

					// Clear the Selected Abilities current Slot (Input Activation Tag)
					//  For the case where it is already assigned to a different Slot (Input Activation Tag)
					ClearBoundAbilityFromSlot(AbilitySpecBoundToSlot);
				}
			}
#pragma endregion

#pragma region SlotIsEmpty
			if (!AbilityBoundToAnySlot(*AbilitySpec))
			{
				// This Ability doesn't have a Slot bound to it (It is not Active/Equipped)

				if (IsPassiveAbility(*AbilitySpec))
				{
					// Activate the Passive Ability
					TryActivateAbility(AbilitySpec->Handle);

					// Activate the Passive Ability effect
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}

			AssignSlotToAbility(*AbilitySpec, SlotToEquip);
#pragma endregion

			MarkAbilitySpecDirty(*AbilitySpec);
		}
		
		// Broadcast the Ability, and Slot (Input Activation Tag) changes to the Client
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, SlotToEquip, PrevSlotInputActivationTag);
	}
}
#pragma endregion

/* Public Client RPC Functions */
#pragma region PublicCLientRPCs
void UAriaAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& AbilityStatus,
                                                                    const FGameplayTag& NewSlot,
                                                                    const FGameplayTag& OldSlot)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, AbilityStatus, NewSlot, OldSlot);
}
#pragma endregion

int32 UAriaAbilitySystemComponent::GetAbilityLevelUpRequirementCost(const FGameplayTag& AbilityTag)
{
	UAbilityInfo* AbilityInfo = UAriaAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		for (const FAriaAbilityInfo& Info : AbilityInfo->AbilityInformation)
		{
			if (Info.AbilityTag.MatchesTagExact(AbilityTag))
			{

					// LevelUpRequirement isn't set;
					//  Just handle without any Level Up Requirements

					// Get the Status from the Ability Spec
					FGameplayTag AbilityStatus = GetAbilityStatusTagFromSpec(*AbilitySpec);
					if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
					{
						// We are unlocking a new Ability
						return AbilitySpec->Level;
					}
					else
					{
						// We likely already have this Ability so just level it up based on the next level's cost requirement
						return AbilitySpec->Level + 1;
					}
			}
		}
	}
	return int32();
}

/* Client RPC Functions */
#pragma region ClientRPCs
void UAriaAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec,
                                                                     FActiveGameplayEffectHandle ActiveEffectHandle)
const
{
	// When any Effect is applied, get all Asset Tags associated with that Effect; loop through and Broadcast them to the Widget Controller
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	// Any class who binds to our EffectAssetTags delegate will receive a Container filled with the Asset Tags.
	EffectAssetTagsDelegate.Broadcast(TagContainer);
}


void UAriaAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
                                                                           const FGameplayTag StatusTag,
                                                                           int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}
#pragma endregion
