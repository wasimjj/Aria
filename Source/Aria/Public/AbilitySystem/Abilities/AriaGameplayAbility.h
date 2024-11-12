// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "AriaGameplayAbility.generated.h"

enum class EPointGenerateSequence : uint8;
enum class EPointGeneratePattern : uint8;
enum class EPointGenerateShape : uint8;

/**
 * @class UAriaGameplayAbility
 *
 * The UAriaGameplayAbility class is a subclass of UGameplayAbility and represents a specific gameplay ability in the game.
 * It provides the functionality to perform certain actions or effects when activated by the player or AI.
 *
 * This class includes a variable named StartupInputTag, which is used to specify the gameplay tag associated with this ability when it is initially given to the character.
 * This variable only applies if the ability is a startup (default) ability and is used for checking at the beginning when initially giving abilities to the character.
 */
UCLASS()
class ARIA_API UAriaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Handles the Current and Next-Level Descriptions for an Ability
	virtual FString GetAbilityDescription(int32 Level);
	virtual FString GetAbilityNextLevelDesc(int32 Level);
	static FString GetAbilityLockedDescription(int32 Level);

	// Note: This variable only really applies if this is a Startup (Default) Ability
	//  (i.e. this is only good to check at the very beginning when initially giving Abilities to the Character)
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	// Determines the Targeting Area for the Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	float TargetingAreaRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bRangedAbility = false;

protected:
	float GetAbilityCost(const float InAbilityLevel = 1.f) const;
	float GetAbilityCooldown(const float InAbilityLevel = 1.f) const;

};
