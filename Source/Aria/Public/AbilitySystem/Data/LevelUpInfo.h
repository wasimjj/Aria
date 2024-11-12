// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAriaLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	// ** Point Reward System **
	// These are specific to those Games where Attribute/Ability points can be
	//  spent for leveling up Character Stats and Abilities
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityPointReward = 0;
};

/**
 * Represents the level up information for a character.
 * Inherits from UDataAsset.
 */
UCLASS()
class ARIA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAriaLevelUpInfo> LevelUpInformation;

	// Function used for getting the Character's Level based on their current XP amount.
	int32 FindLevelBasedOnTotalXP(int32 XPAmount) const;
};
