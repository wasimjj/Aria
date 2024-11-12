// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

// This Struct stores all the information associated with a given Ability
//   This is primarily used currently by the UI HealthManaAbilities Widget Blueprint

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAriaAbilityInfo
{
	GENERATED_BODY()
	
	// This Gameplay Tag will be used to uniquely identify an Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	// This Gameplay Tag will be used to to help categorize the Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	// This Dynamic Gameplay Tag will be used to track the Status of the Ability
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityStatus = FGameplayTag();

	// The Level Requirement for using/unlocking the Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;
	
	// The Item/KeyItem Requirement for using/unlocking the Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemRequirement = 0;

	// Used for those Games where Abilities get automatically triggered via Button Inputs from the Player
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputActivationTag = FGameplayTag();
	
	// Cooldown Tag associated with the Activated Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	/* UI-related */
#pragma region UI
	//** UI > Ability Globe-related properties **
	// The Image that is used to fill an AbilityGlobe
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> AbilityGlobeMaterial = nullptr;

	// The icon that shows on the AbilityGlobe
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> AbilityGlobeIcon = nullptr;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;
};

/**
 * UAbilityInfo is a data asset class that stores information about abilities.
 * It is used to provide ability information to other systems, such as UI widgets or gameplay ability components.
 */
UCLASS()
class ARIA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information")
	TArray<FAriaAbilityInfo> AbilityInformation;

	FAriaAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, const bool bLogNotFound = false) const;
};
