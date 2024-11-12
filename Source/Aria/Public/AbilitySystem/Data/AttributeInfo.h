// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

// This Struct stores all of the information associated with a given Attribute
//   This Struct will be Broadcast up to the Widget Blueprints whenever an Attribute changes.
//   This is primarily used currently by the UI > AttributeMenu Widget Blueprint

USTRUCT(BlueprintType)
struct FAriaAttributeInfo
{
	GENERATED_BODY()

	// The Gameplay Tag will be used to help identify Attributes.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	// FText is needed for any Text that will be user-facing (setting the Text for a widget that gets shown to the User) in a widget. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// Note: We do not want to be able to edit the AttributeValue directly on the DataAsset
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * @class UAttributeInfo
 *
 * UAttributeInfo is a data asset class that contains information about Aria attributes.
 * The class inherits from UDataAsset, which allows it to be created as an asset in the content browser.
 * It contains an array of FAriaAttributeInfo structs that hold information about individual attributes.
 *
 * @see UDataAsset
 * @see FAriaAttributeInfo
 */
UCLASS()
class ARIA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// This will contain all of the Aria Attribute Info structs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAriaAttributeInfo> AttributeInformation;

	// Function takes a Attribute Gameplay Tag and returns the correct info based on it.
	FAriaAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, const bool bLogNotFound = false) const;
};
