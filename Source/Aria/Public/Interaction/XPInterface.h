// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XPInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXPInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * \class IXPInterface
 *
 * \brief The interface for handling experience points (XP) and level up functionality.
 *
 * The IXPInterface class defines the interface for handling experience points (XP) and level up functionality.
 * It provides methods for retrieving the total XP, finding the level based on the XP total,
 * adding XP to the total, performing level up functions, and managing attribute and ability points.
 *
 * To implement the IXPInterface, inherit from this class and implement the required blueprint native events.
 *
 * Usage example:
 * \code
 * class MyClass : public IXPInterface
 * {
 *     GENERATED_BODY()
 *
 * public:
 *     // Implement the blueprint native events
 * };
 * \endcode
 */
class ARIA_API IXPInterface
{
	GENERATED_BODY()

public:
	// The total Experience Points that the Player, Character or Object has accrued throughout the game
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXPTotal() const;

	// Retrieve the current Level based on XP total 
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXPTotal(int32 InXPAmount) const;

	// Adds an additional XP amount to the current XP total
	UFUNCTION(BlueprintNativeEvent)
	void AddXPToXPTotal(int32 InXPAmount);

	// Increments Level by a specified amount
	UFUNCTION(BlueprintNativeEvent)
	void AddToLevel(const int32 InLevelAmtToAdd);
	
	// Performs aesthetic & cosmetic Level Up functions only; such as triggering the LevelUp VFX and SFX
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	
	/* Attribute Points System */
#pragma region Attribute Points System
	// Used for Games that give Attribute Bonuses on Level Up
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 InLevel) const;
	// Used for Games that give Attribute Bonuses on Level Up
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints();
	// Used for Games that give Attribute Bonuses on Level Up
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);
#pragma endregion

	/* Ability Points System */
#pragma region Ability Points System
	// Used for Games that give Ability Point Bonuses on Level Up
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPointsReward(const int32 InLevel) const;
	// Used for Games that give Ability Point Bonuses on Level Up
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPoints();
	// Used for Games that give Ability Point Bonuses on Level Up
	UFUNCTION(BlueprintNativeEvent)
	void AddToAbilityPoints(int32 InAbilityPoints);
#pragma endregion
};
