// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AriaInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAriaInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * \brief The UAriaInputConfig class is a data asset that stores a list
 * of Ability Input Actions that are linked to Gameplay Tags.
 *
 * This class inherits from UDataAsset and is part of the Aria API.
 * It is used to define the input configuration for abilities in the game.
 * The Ability Input Actions are stored in a TArray<FAriaInputAction>.
 */
UCLASS()
class ARIA_API UAriaInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// Returns an Input Action associated with a given Tag
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	                                                 const bool bLogNotFound = false) const;

	// Stores a list of Ability Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAriaInputAction> AbilityInputActions;
};
