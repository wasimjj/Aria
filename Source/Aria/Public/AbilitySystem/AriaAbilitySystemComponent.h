// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AriaAbilitySystemComponent.generated.h"

// DataTable Row Struct for Ability Descriptions
// Stores information used for displaying the Descriptions of an Ability at their various Levels
USTRUCT(BlueprintType)
struct FUIAbilityDescriptionsRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AbilityLevel = int32();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText AbilityDescription = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText AbilityNextLevelDescription = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText AbilityLockedDescription = FText();
};

struct FAriaAbilityInfo;
// AbilitySystemComponent needs to be able to Broadcast this Delegate
// WidgetController needs to be able to Bind to this Delegate
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /* AssetTags*/)

// Delegate that broadcasts out to anyone listening for whenever Abilities have been given (added) to the ASC
// Setting to Multicast; In the case where multiple classes may need to bind to it
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);

// Delegate used to loop through each Ability (i.e. Ability Spec)
DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);

// Delegate that broadcasts out an Ability Status changed to anyone listening
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedSignature, const FGameplayTag& /*AbilityTag*/,
                                       const FGameplayTag& /*StatusTag*/, const int32 /*AbilityLevel*/);

// Delegate that broadcasts out whenever an Ability has been Equipped
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquippedSignature, const FGameplayTag& /*AbilityTag*/,
                                      const FGameplayTag& /*StatusTag*/, const FGameplayTag& /*NewSlot*/,
                                      const FGameplayTag& /*OldSlot*/);

// Delegate that listens for whenever something is attempting to Deactivate a Passive Ability
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbilitySignature, const FGameplayTag& /*PassiveAbilityTag*/);

// Delegate that listens for whenever something is attempting to Activate a Passive Ability Effect (i.e. turns on/off the Passive Ability Niagara Component)
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffectSignature, const FGameplayTag& /*PassiveAbilityTag*/, bool /*bActivate*/);


/**
 * @class UAriaAbilitySystemComponent
 *
 * The UAriaAbilitySystemComponent class is a subclass of UAbilitySystemComponent.
 * It provides additional functionality and utilities for managing gameplay abilities and attributes
 * for a game character or enemy.
 *
 * @public
 *
 * @brief Provides functionality for managing gameplay abilities and attributes for a game character or enemy.
 *
 * @details The UAriaAbilitySystemComponent class extends the UAbilitySystemComponent class and provides
 * additional functionality for managing gameplay abilities and attributes for a game character or enemy.
 * 
 * It includes methods for initializing ability actor info, adding character abilities, activating abilities,
 * updating attributes, and more.
 *
 * @note This class is intended to be used as a base class and should be subclassed to add custom functionality
 * specific to the game project.
 *
 * @see UAbilitySystemComponent
 *
 * @note This class should not be instantiated directly. Instead, it should be subclassed and instantiated
 * using the derived class.
 *
 * @see UAriaAbilitySystemComponentExample (example subclass)
 *
 * @note This class is not thread-safe and should be used only on the game thread.
 *
 * @note This class is part of the this game project and is not intended for general use in other projects.
 *
 */
UCLASS()
class ARIA_API UAriaAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTagsSignature EffectAssetTagsDelegate;
	FAbilitiesGivenSignature AbilitiesGivenDelegate;
	FAbilityStatusChangedSignature AbilityStatusChangedDelegate;
	FAbilityEquippedSignature AbilityEquippedDelegate;
	FDeactivatePassiveAbilitySignature DeactivatePassiveAbilityDelegate;
	FActivatePassiveEffectSignature ActivatePassiveEffectDelegate;
	
	bool bStartupAbilitiesGiven = false;

	/* Ability-based Functions */
#pragma region Ability Functions
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InitialAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InitialPassiveAbilities);

	// This is used to loop through all of its Activatable Ability
	void ForEachAbility(const FForEachAbilitySignature& AbilityDelegate);

	// These functions are specifically used for Activating Abilities
	void AscAbilityInputTagPressed(const FGameplayTag& InputTag);
	void AscAbilityInputTagHeld(const FGameplayTag& InputTag);
	void AscAbilityInputTagReleased(const FGameplayTag& InputTag);

	// Function used for Unlocking all of the Abilities in Ability Info (based on the current Level)
	void UnlockAbilitiesByLevel(const int32 Level);

	// Function used for Updating the Eligibility of all Abilities in Ability Info (based on the current Level)
	void UpdateAbilitiesEligibilityByLevel(const int32 Level);

	// ** Used with the Ability Equipping System **
#pragma region AbilityEquipSystem
	
	// Checks whether the Slot is Empty
	bool SlotIsEmpty(const FGameplayTag& Slot);
	
	// Function for checking whether the Ability is currently equipped in the Slot 
	static bool AbilityBoundToSlot(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& Slot);
	
	// Function for checking whether the Ability is currently equipped in Any Slot 
	static bool AbilityBoundToAnySlot(const FGameplayAbilitySpec& AbilitySpec);
	
	// Gets the Ability Spec associated with the current Slot
	FGameplayAbilitySpec* GetAbilitySpecBoundToSlot(const FGameplayTag& Slot);
	
	// Check whether the Ability is a Passive Ability
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const;
	
	// Function used to assign a Slot to an Ability
	static void AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& Slot);
	
	// This function clears the Slot (Input Activation Tag) from the Ability associated with it
	static void ClearBoundAbilityFromSlot(FGameplayAbilitySpec* AbilitySpec);
	
#pragma endregion

	// Used for Activating/Deactivating Passive Ability Effects (such as the Passive Niagara Component)
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& PassiveAbilityTag, bool bActivate);
	
#pragma endregion

	/* Attribute-based Functions */
#pragma region Attribute Functions
	// Function used for Updating AttributeSet.Attributes in the Ability System
	void UpdateAttribute(const FGameplayTag& AttributeTag);

	//  This function will run on the Server only (Server RPC)
	UFUNCTION(Server, Reliable)
	void ServerUpdateAttribute(const FGameplayTag& AttributeTag);
#pragma endregion

	// ** GetFromAbilityTag Functions
#pragma region GetFromAbilityTag_Functions
	FGameplayTag GetAbilityStatusFromAbilityTag(const FGameplayTag& AbilityTag);

	// Function returns the Abilities Description(s) where available
	bool GetAbilityDescriptionsFromAbilityTag(const FGameplayTag& AbilityTag, FText& OutAbilityDescription,
	                                          FText& OutAbilityNextLevelDesc);

	FGameplayTag GetInputActivationTagFromAbilityTag(const FGameplayTag& AbilityTag);
#pragma endregion

	// ** GetFromSpec Functions **
#pragma region GetFromSpec_Functions
	// Function used for lookup of an Ability Tag from the Ability Spec
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	// Function used for lookup of an Ability Type from the Ability Spec
	static FGameplayTag GetAbilityTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	// Function used for lookup of an Ability Status from the Ability Spec
	static FGameplayTag GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	// Function used for lookup of an Input Activation Tag from the Ability Spec
	static FGameplayTag GetInputActivationTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	// Function used for returning a Gameplay Spec pointer based on the passed in Ability Tag
	//  Note: This only gets returned IF the Asc has an Ability with that Ability Tag
	//         So if an Ability hasn't been given to its ASC, then a null ptr will
	//          be returned instead (meaning we don't have an Ability with that Tag)
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
#pragma endregion

	/* Public Server RPC Functions */
#pragma region PublicServerRPCs
	UFUNCTION(Server, Reliable)
	void ServerAbilityPointsSpent(const FGameplayTag& AbilityTag, const bool bRestartAbility = false);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& EquipSlot);
#pragma endregion

	/* Public Client RPC Functions */
#pragma region PublicCLientRPCs
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus,
	                        const FGameplayTag& NewSlot, const FGameplayTag& OldSlot);
#pragma endregion

protected:
	virtual void OnRep_ActivateAbilities() override;

	/* Client RPC Functions */
#pragma region ClientRPCs
	// Fix: Added UFUNCTION(Client, Reliable) to make this a Client RPC (meaning this function will work on all Clients as well)
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                         FActiveGameplayEffectHandle ActiveEffectHandle) const;

	// Client RPC for Broadcast Ability Status changes to Clients
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag StatusTag, int32 AbilityLevel);
#pragma endregion

private:
	int32 GetAbilityLevelUpRequirementCost(const FGameplayTag& AbilityTag);
};
