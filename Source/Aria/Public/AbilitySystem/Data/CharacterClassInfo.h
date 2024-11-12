// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UAriaGameplayAbility;
class UGameplayEffect;

// The Character class types
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	// The pet class
	Summoner,
	// The Sorcerer/Wizardess class
	Elementalist,	
	// The Ranged weapon expert
	Ranger,
	// The Melee Sword and Shield expert
	Warrior
};

// Contains all of the information associated with a given class
USTRUCT(BlueprintType)
struct FCharacterClassData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> ResistancesAttributes;
	
	// This contains the Class-specific Abilities that each Character Class will start with
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> ClassAbilities;

	// This controls the amount of XP a class (or monster class family) will give based on specified curve in the XP curve table
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * UCharacterClassInfo is a class that provides information about character classes in the game.
 * It is a subclass of UDataAsset and allows the editor to specify default values for character class information.
 *
 * The CharacterClassInformation property is a map that links a class enum to a struct that contains data about that class.
 * This allows for easy retrieval of class information based on the class type.
 *
 * The SecondaryAttributes and VitalAttributes properties are subclasses of UGameplayEffect that define the secondary and vital attributes common to all character classes.
 * These can be used to apply the same gameplay effects to all characters of any class.
 *
 * The CommonAbilities property is an array of subclasses of UGameplayAbility that represents the abilities that are common to all classes.
 * These can include abilities such as hit reactions that are shared by all characters.
 *
 * The DamageCalculationCoefficients property holds a table of coefficients used in calculating damage.
 * This can be used to customize the damage formula for different character classes.
 *
 * The GetCharacterClassInfo function allows retrieval of the character class information based on the passed-in class type.
 */
UCLASS()
class ARIA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	// Map for linking a class enum to a character class info struct type.
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassData> CharacterClassInformation;

	// Secondary and Vital Attributes common for any Character classes
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	// These are abilities that are common to all classes (such as the Hit React ("GA_HitReact") gameplay ability)
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	// This holds a table of Coefficients used in calculating Damage
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
	
	// Function returns the Character Class Information based on the passed in Class Type
	FCharacterClassData GetCharacterClassInfo(const ECharacterClass CharacterClass);
};
