// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * The FAriaGameplayTags class represents a collection of gameplay tags used in the game system.
 * It contains tags related to input, gameplay abilities, gameplay effects, montages, combat sockets, equipped items,
 * positive/negative/neutral status effects, damage types, attributes (including meta attributes), and more.
 *
 * The class provides a static Get() function that returns an instance of the FAriaGameplayTags class.
 *
 * The InitializeNativeGameplayTags() function is used to create native gameplay tags.
 *
 * The class has several public member variables that represent the different gameplay tags.
 *
 * The FAriaGameplayTags class is a simple data structure and does not contain any methods or business logic.
 */

struct FAriaGameplayTags
{
public:
	static const FAriaGameplayTags& Get() { return GameplayTags; }

	// This function is used to create those native Gameplay Tags
	static void InitializeNativeGameplayTags();

	/* Player Tags */
#pragma region Player Tags
	// These are tags related to the Player, Player Controller, or Player State

	/* Block Tags */
	//  These Tags are used for blocking actions that a Player tries to perform (ex: Blocking the Player Input)
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
#pragma endregion
	
	/* Input Tags */
#pragma region Input Tags
	FGameplayTag InputTag_LMB; // Left Mouse Button
	FGameplayTag InputTag_RMB; // Right Mouse Button
	FGameplayTag InputTag_1; // Top-side Keyboard #1 Key
	FGameplayTag InputTag_2; // Top-side Keyboard #2 Key
	FGameplayTag InputTag_3; // Top-side Keyboard #3 Key
	FGameplayTag InputTag_4; // Top-side Keyboard #4 Key

	FGameplayTag InputTag_Passive_1; // Passive Ability Slot 1 Input Trigger
	FGameplayTag InputTag_Passive_2; // Passive Ability Slot 2 Input Trigger
#pragma endregion

	/* Gameplay Abilities Tags (includes Ability Type, Ability Status, and Ability Cooldown) */
#pragma region Gameplay Abilities Tags

	// This Ability Tag acts as a catch-all; needed for different situations
	FGameplayTag Abilities_None;

	/* == Ability Type Tags == */

	// Typically handled by the System and not available for the Player to use;
	//  (Ex: HitReact Gameplay Ability that controls any reactions a Character does after being hit)
	FGameplayTag Abilities_Type_System;
	// Offensive Abilities used by the Player or other Characters
	//  (Ex: Firebolt or Icebolt ability)
	FGameplayTag Abilities_Type_Offensive;
	// Defensive Abilities used by the Player or other Characters
	//  (Ex: Shield spell to reduce Physical Damage)
	FGameplayTag Abilities_Type_Defensive;
	// Restorative Abilities used by the Player or other Characters
	//  (Ex: Healing spell to restore Health or Revive spell to resurrect a Player or Character)
	FGameplayTag Abilities_Type_Restorative;
	// Passive Abilities used by the Player or other Characters that remain active over time until removed
	FGameplayTag Abilities_Type_Passive;

	/* == Ability Status Tags == */

	// Ability is locked and unavailable until meeting certain Criteria
	//  Scenario 1: Reaching a certain Level or spending available points to unlock the Ability; or both
	//  Scenario 2: Finding a Key Item used to unlock the Ability
	//  Scenario 3: Reaching a key point in the Game
	FGameplayTag Abilities_Status_Locked;
	// Typically used in Games where Points are spent to Unlock new Abilities
	FGameplayTag Abilities_Status_Eligible;
	// Ability becomes available for use
	FGameplayTag Abilities_Status_Unlocked;
	// Used in Games where Abilities are manually Set and Triggered; typically through Player KBM/Controller Inputs
	FGameplayTag Abilities_Status_Equipped;

	/* == System Ability Tags == */
	FGameplayTag Abilities_System_Attack;
	FGameplayTag Abilities_System_Summon;
	FGameplayTag Abilities_System_HitReact;

	/* == Offensive Ability Tags == */
	FGameplayTag Abilities_Offensive_Fire_Firebolt;
	FGameplayTag Abilities_Cooldown_Offensive_Fire_Firebolt;
	FGameplayTag Abilities_Offensive_Lightning_Shock;
	FGameplayTag Abilities_Cooldown_Offensive_Lightning_Shock;
	FGameplayTag Abilities_Offensive_Lightning_Storm;
	FGameplayTag Abilities_Cooldown_Offensive_Lightning_Storm;
	FGameplayTag Abilities_Offensive_Arcane_ArcaneShards;
	FGameplayTag Abilities_Cooldown_Offensive_Arcane_ArcaneShards;

	/* == Defensive Ability Tags == */


	/* == Restorative Ability Tags == */


	/* == Passive Ability Tags == */
	// Applies the Barrier Status Effect to the Character
	FGameplayTag Abilities_Passive_Barrier;
	
	// Instantly restores Health by stealing it from other Character(s)
	FGameplayTag Abilities_Passive_SiphonHealth;
	// Instantly restores Mana by stealing it from other Character(s)
	FGameplayTag Abilities_Passive_SiphonMana;
	// Instantly restores Stamina by stealing it from other Character(s)
	FGameplayTag Abilities_Passive_SiphonStamina;
#pragma endregion

	/* Gameplay Effects Tags */
#pragma region Gameplay Effects Tags
	FGameplayTag Effects_HitReact;
#pragma endregion

	/* Montage Tags */
#pragma region Montage Tags
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	FGameplayTag Montage_Attack_5;
	FGameplayTag Montage_Summoning;
#pragma endregion

	/* Combat Socket Tags */
#pragma region Combat Socket Tags
	// Note: These Combat Socket Tags are used to determine Effect Spawn Locations and even Damage Impact Locations
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	FGameplayTag CombatSocket_Head;
#pragma endregion

	/* Equipped Item Tags */
#pragma region Equipped Item Tags
	FGameplayTag Equipped_Weapon_OneHanded;
	FGameplayTag Equipped_Weapon_TwoHanded;
	FGameplayTag Equipped_Weapon_Bow;
	FGameplayTag Equipped_Weapon_Crossbow;
	FGameplayTag Equipped_Weapon_Throwing;
#pragma endregion

	/* Status Effect Tags */
#pragma region Status Effects

	// Determines the Success % that a Status Effect with be applied
	FGameplayTag StatusEffect_Chance;
	// Determines how long the Status Effect will remain Active
	//  Note: This can also be a Random interval if desired
	FGameplayTag StatusEffect_Duration;
	// Determines how often to trigger the Status Effect (Acts as the Period in a Gameplay Effect)
	FGameplayTag StatusEffect_Frequency;
	// Determines the Damage Amount (where applicable) to apply per interval
	FGameplayTag StatusEffect_Damage;

	/* Negative Status Effects Tags */
#pragma region Negative Status Effects

	// * Means they can't be removed at all
	// ** Means they can't be removed except by special means

	// * The Character is very briefly unable to move or perform any Actions for a short period of time
	FGameplayTag StatusEffect_Negative_Stun;

	// * The Character will receive damage over time for a set period of time (Note: This may or may not include Reduced Defense)
	FGameplayTag StatusEffect_Negative_Burning;
#pragma endregion

#pragma endregion

	/* Damage Type Tags */
#pragma region Damage Type Tags
	// Applies Physical Damage
	//  (i.e. Melee Damage and Ranged damage from Arrows, Crossbow Bolts, Throwing Weapons, etc.)
	FGameplayTag DamageType_Physical;
	FGameplayTag DamageType_Arcane; // Applies Non-Elemental Damage
	FGameplayTag DamageType_Fire; // Applies Fire Damage
	FGameplayTag DamageType_Lightning; // Applies Lightning (or Electricity) Damage
#pragma endregion

	/* Resistance Tags */
#pragma region Resistance Tags

	/* Damage Type Resistances */
	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;

	/* Status Type Resistances */
	FGameplayTag Attributes_Resistance_Stun;
	FGameplayTag Attributes_Resistance_Burning;
#pragma endregion

	/* Relationship Maps (DamageTypesToResistances) / (StatusEffectTypesToResistances) / etc. */
#pragma region RelationshipMaps
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> StatusEffectsToResistances;
#pragma endregion

	/* Primary Attributes */
#pragma region Primary Attributes
	/*
	 * Increases Melee and Ranged Attack Damage.
	 *	Notes:
	 *		Base Damage = The difference between the Attacker's Str and the Defender's Res.
	 *		When using a Single-hand Weapon or Ranged Weapon:
	 *			For every 2 points of Str, a Character's Attack power is raised by 1 (.50 per one point of Str).
	 *		When using a 2-handed Weapon:
	 *			For every 4 points of Str, a Character's Attack Power is raised by 3 (.75 per one point of Str).
	 */
	FGameplayTag Attributes_Primary_Strength;

	/*
	 * Increases Melee and Ranged Weapon Accuracy.
	 *	Notes:
	 *		Base Critical Hit Rate = The Difference between the Attacker's Dex and the Defender's Agi.
	 *			(The higher a Defender's Agility, the less likely they are to have Critical Hits scored against them.)
	 *		When using a Single-hand Weapon or Ranged Weapon:
	 *			For every 2 points of Dex, a Character's Accuracy is raised by 1 (.50 per one point of Dex).
	 *		When using a 2-handed Weapon:
	 *			For every 4 points of Dex, a Character's Accuracy is raised by 3 (.75 per one point of Dex).
	 */
	FGameplayTag Attributes_Primary_Dexterity;

	/*
	 * Increases Defense against Physical Attacks.
	 *	Notes:
	 *		Base Physical Damage Taken = The difference between the Attacker's Str and the Defender's Res.
	 *		For every 2 points of Res, a Character's Defense is raised by 1 (.50 per one point of Res).
	 */
	FGameplayTag Attributes_Primary_Resilience;

	/*
	 * Increases Evasion from Physical Attacks.
	 *	Notes:
	 *		Base Critical Hit Rate = The Difference between the Attacker's Dex and the Defender's Agi.
	 *			(The higher a Defender's Agility, the less likely they are to have Critical Hits scored against them.)
	 *		For every 2 points of Agi, a Character's Evasion is raised by 1 (.50 per one point of Agi).
	 */
	FGameplayTag Attributes_Primary_Agility;

	/*
	 * Increases Healing Magic Potency and Improves Resistance to Status Ailments.
	 *	Notes:
	 *		xx
	 */
	FGameplayTag Attributes_Primary_Spirit;

	/*
	 * Increases Attack Magic Potency and Accuracy.
	 * Also increases Maximum Mana
	 *	Notes:
	 *		xx
	 */
	FGameplayTag Attributes_Primary_Intelligence;

	/*
	 * Increases Maximum Health
	 *	Notes:
	 *		Max Health = 16.5 x Vit
	 */
	FGameplayTag Attributes_Primary_Vitality;

	/*
	 * Slightly increases Critical Hit Chance and opportunities to evade Status Ailments.
	 *	Notes:
	 *		xx
	 */
	FGameplayTag Attributes_Primary_Luck;

	/*
	 * Increases Health Regeneration.
	 *	Notes:
	 *		xx
	 */
	FGameplayTag Attributes_Primary_Endurance;

	/*
	 * Increases Mana Regeneration.
	 *	Notes:
	 *		xx
	 */
	FGameplayTag Attributes_Primary_Willpower;
#pragma endregion

	/* Secondary Attributes */
#pragma region Secondary Attributes
	/*
	 * The Maximum Health of the Character/Player.
	 * Formula:
	 *	Vit * ?
	 */
	FGameplayTag Attributes_Secondary_MaxHealth;

	/*
		* The Maximum Mana of the Character/Player.
	 * Formula:
	 *	Int * ?
	 */
	FGameplayTag Attributes_Secondary_MaxMana;

	/*
	 * The Physical Attack Damage of the Character/Player.
	 * Formula:
	 *	Str/Agi (depending on Class/or Weapon selected) * ?
	 */
	FGameplayTag Attributes_Secondary_Attack;

	/*
	 * Reduces Damage taken from Physical Attacks. 
	 * Formula:
	 *	Res * ?
	 */
	FGameplayTag Attributes_Secondary_Defense;

	/*
	 * Chance to completely Negate Damage from a Physical Attack.
	 * Formula:
	 *	Agi * Luck * ?
	 */
	FGameplayTag Attributes_Secondary_DodgeChance;

	/*
	 * Chance to reduce Damage from a Physical Attack by a certain amount.
	 * Formula:
	 *	Def * Luck * ?
	 */
	FGameplayTag Attributes_Secondary_BlockChance;

	/*
	 * The Magical Attack Damage of the Character/Player.
	 * Formula:
	 *	Int * ?
	 */
	FGameplayTag Attributes_Secondary_MagicalAttack;

	/*
	 * Reduces Magical Damage taken from Magical Attacks.
	 * Formula:
	 *	Spi * ?
	 */
	FGameplayTag Attributes_Secondary_MagicalDefense;

	/*
	 * Ignores Enemy Defense.
	 * Formula:
	 *	Res * ?
	 */
	FGameplayTag Attributes_Secondary_ArmorPenetration;

	/*
	 * Chance for additional Bonus Damage to be applied based on the Critical Damage Amount.
	 * Formula:
	 *	ArmPen * Luck * ?
	 */
	FGameplayTag Attributes_Secondary_CriticalHitChance;

	/*
	 * The amount of Bonus Damage to apply.
	 * Formula:
	 *	Attack * ArmPen * ?
	 */
	FGameplayTag Attributes_Secondary_CriticalHitDamage;

	/*
	 * Reduces the Critical Hit Chance of attacking Enemies.
	 * Formula:
	 *	Def * Luck * ?
	 */
	FGameplayTag Attributes_Secondary_CriticalHitResistance;

	/*
	 * Slowly restores the Health of the Character/Player per Tick.
	 * Formula:
	 *	Vit * ?
	 */
	FGameplayTag Attributes_Secondary_HealthRegeneration;

	/*
	 * Slowly restores the Mana of the Character/Player per Tick.
	 * Formula:
	 *	Int * ?
	 */
	FGameplayTag Attributes_Secondary_ManaRegeneration;
#pragma endregion

	/* Vital Attributes */
#pragma region Vital Attributes
	/*
	 * Amount of Damage a Character/Player can take before Death.
	 */
	FGameplayTag Attributes_Vital_Health;

	/*
	 * A Resource used in the Casting of Spells and Abilities.
	 */
	FGameplayTag Attributes_Vital_Mana;

	/*
	 * A Resource used for Running, Swimming, Climbing, or other energy-draining activities.
	 */
	FGameplayTag Attributes_Vital_Stamina;
#pragma endregion

	/* Meta Attributes */
#pragma region Meta Attributes
	FGameplayTag Attributes_Meta_IncomingDamage;
	FGameplayTag Attributes_Meta_IncomingXP;
#pragma endregion

private:
	static FAriaGameplayTags GameplayTags;
};
