// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AriaGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UDataTable;

/**
 * @class AAriaGameModeBase
 * @brief The game mode base class for the game.
 *
 * This class represents the base game mode for the game.
 * It is a subclass of AGameModeBase.
 *
 * @see AGameModeBase
 */
UCLASS()
class ARIA_API AAriaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities | Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// Contains a data table with all of the Ability Description data used in the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities | Ability Descriptions")
	TObjectPtr<UDataTable> AbilityDescriptionsDataTable;
};
