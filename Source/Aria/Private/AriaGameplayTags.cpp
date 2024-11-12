// Copyright Savoie Interactive Studios


#include "AriaGameplayTags.h"
#include "GameplayTagsManager.h"

FAriaGameplayTags FAriaGameplayTags::GameplayTags;

void FAriaGameplayTags::InitializeNativeGameplayTags()
{
	/* Player Tags */
#pragma region Player Tags
	/* Block Tags */
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Player.Block.InputPressed"), "Block Player Input - Input Pressed");
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Player.Block.InputHeld"), "Block Player Input - Input Held");
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Player.Block.InputReleased"), "Block Player Input - Input Released");
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Player.Block.CursorTrace"), "Block Player Input - Tracing under the Cursor");
#pragma endregion
	
	/* Input Tags */
#pragma region Input Tags
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"), "Input Tag for Left Mouse Button");
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"), "Input Tag for Right Mouse Button");
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"), "Input Tag for Top-side Keyboard #1 Key");
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"), "Input Tag for Top-side Keyboard #2 Key");
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"), "Input Tag for Top-side Keyboard #3 Key");
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"), "Input Tag for Top-side Keyboard #4 Key");

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"), "Passive Ability Slot 1 Input Trigger");
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"), "Passive Ability Slot 2 Input Trigger");
#pragma endregion

	/* Gameplay Abilities Tags (includes Ability Type, Ability Status, and Ability Cooldown) */
#pragma region Gameplay Abilities Tags

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"), "Ability None Tag (No Ability; Acts like a Catch-all / nullptr-acting)");

	// Ability Type Tags
	GameplayTags.Abilities_Type_System = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.System"), "Ability System Type Tag");
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"), "Ability Offensive Type Tag");
	GameplayTags.Abilities_Type_Defensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Defensive"), "Ability Defensive Type Tag");
	GameplayTags.Abilities_Type_Restorative = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Restorative"), "Ability Restorative Type Tag");
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"), "Ability Passive Type Tag");

	// Ability Status Tags
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"), "Ability Locked Status Tag");
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"), "Ability Eligible Status Tag");
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"), "Ability Unlocked Status Tag");
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"), "Ability Equipped Status Tag");

	// System Ability Tags
	GameplayTags.Abilities_System_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.System.Attack"), "Tag granted when Attacking");
	GameplayTags.Abilities_System_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.System.Summon"), "Tag granted when Summoning");
	GameplayTags.Abilities_System_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.System.HitReact"), "Tag granted when Hit Reacting");

	// Offensive Ability Tags
	GameplayTags.Abilities_Offensive_Fire_Firebolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Offensive.Fire.Firebolt"), "Firebolt Ability Tag");
	GameplayTags.Abilities_Cooldown_Offensive_Fire_Firebolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Cooldown.Offensive.Fire.Firebolt"), "Firebolt Cooldown Tag");
	GameplayTags.Abilities_Offensive_Lightning_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Offensive.Lightning.Shock"), "Shock Ability Tag");
	GameplayTags.Abilities_Cooldown_Offensive_Lightning_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Cooldown.Offensive.Lightning.Shock"), "Shock Cooldown Tag");
	GameplayTags.Abilities_Offensive_Lightning_Storm = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Offensive.Lightning.Storm"), "Storm Ability Tag");
	GameplayTags.Abilities_Cooldown_Offensive_Lightning_Storm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Cooldown.Offensive.Lightning.Storm"), "Storm Cooldown Tag");
	GameplayTags.Abilities_Offensive_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Offensive.Arcane.ArcaneShards"), "Arcane Shards Ability Tag");
	GameplayTags.Abilities_Cooldown_Offensive_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Cooldown.Offensive.Arcane.ArcaneShards"), "Arcane Shards Cooldown Tag");

	// Defensive Ability Tags


	// Restorative Ability Tags


	// Passive Ability Tags
	GameplayTags.Abilities_Passive_Barrier = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Barrier"), "Passive Ability Tag");
	GameplayTags.Abilities_Passive_SiphonHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.SiphonHealth"), "Passive Ability Tag");
	GameplayTags.Abilities_Passive_SiphonMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.SiphonMana"), "Passive Ability Tag");
	GameplayTags.Abilities_Passive_SiphonStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.SiphonStamina"), "Passive Ability Tag");
#pragma endregion

	/* Gameplay Effects Tags */
#pragma region Gameplay Effects Tags
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"), "Tag granted when Hit Reacting");
#pragma endregion

	/* Montage Tags */
#pragma region Montage Tags
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"), "Attack 1");
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"), "Attack 2");
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"), "Attack 3");
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"), "Attack 4");
	GameplayTags.Montage_Attack_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.5"), "Attack 5");
	GameplayTags.Montage_Summoning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Summoning"), "Using a Summoning Ability");
#pragma endregion

	/* Combat Socket Tags */
#pragma region Combat Socket Tags
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"), "Weapon Socket");
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"), "Right Hand Socket");
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"), "Left Hand Socket");
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"), "Tail Socket");
	GameplayTags.CombatSocket_Head = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Head"), "Head Socket");
#pragma endregion

	/* Equipped Item Tags */
#pragma region Equipped Item Tags
	GameplayTags.Equipped_Weapon_OneHanded = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Equipped.Weapon.OneHanded"), "One-Handed Weapon Currently Equipped");
	GameplayTags.Equipped_Weapon_TwoHanded = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Equipped.Weapon.TwoHanded"), "Two-Handed Weapon Currently Equipped");
	GameplayTags.Equipped_Weapon_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Equipped.Weapon.Bow"), "Bow Currently Equipped");
	GameplayTags.Equipped_Weapon_Crossbow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Equipped.Weapon.Crossbow"), "Crossbow Currently Equipped");
	GameplayTags.Equipped_Weapon_Throwing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Equipped.Weapon.Throwing"), "Throwing Weapon Currently Equipped");
#pragma endregion
	
	/* Status Effect Tags */
#pragma region Status Effects

	GameplayTags.StatusEffect_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffect.Chance"), "Determines the Success % that a Status Effect with be applied");
	GameplayTags.StatusEffect_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffect.Duration"),
		"Determines how long (using set or random interval) the Status Effect will remain Active");
	GameplayTags.StatusEffect_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffect.Frequency"),
		"Determines how often to trigger the Status Effect (Acts as the Period in a Gameplay Effect)");
	GameplayTags.StatusEffect_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffect.Damage"), "Determines the Damage Amount (where applicable) to apply per interval");

	/* Negative Status Effects Tags */
#pragma region Negative Status Effects
	GameplayTags.StatusEffect_Negative_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffect.Negative.Stun"),
		"The Character is very briefly unable to move or perform any Actions for a short period of time");
	GameplayTags.StatusEffect_Negative_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffect.Negative.Burning"),
		"The Character will receive damage over time for a set period of time (Note: This may or may not include Reduced Defense)");
#pragma endregion


#pragma endregion

	/* Damage Type Tags */
#pragma region Damage Type Tags
	GameplayTags.DamageType_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Physical"),
		"Applies Physical Damage (i.e. Melee Damage and Ranged damage from Arrows, Crossbow Bolts, Throwing Weapons, etc.)");
	GameplayTags.DamageType_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Arcane"), "Applies Non-Elemental Damage");
	GameplayTags.DamageType_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Fire"), "Applies Fire Damage");
	GameplayTags.DamageType_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Lightning"), "Applies Lightning (or Electricity) Damage");
#pragma endregion

	/* Resistance Tags */
#pragma region Resistance Tags

	/* Damage Type Resistances */
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"), "Adds resistance to Physical");
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"), "Adds resistance to Arcane");
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"), "Adds resistance to Fire");
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"), "Adds resistance to Lightning (and Electricity)");
	GameplayTags.Attributes_Resistance_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Stun"), "Adds resistance to Stun");
	GameplayTags.Attributes_Resistance_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Burning"), "Adds resistance to Burning");

#pragma endregion

	/* Relationship Maps (DamageTypesToResistances) / (StatusEffectTypesToResistances) / etc. */
#pragma region RelationshipMaps
	/* DamageTypesToResistances */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Physical,
	                                          GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.
	             Add(GameplayTags.DamageType_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Lightning,
	                                          GameplayTags.Attributes_Resistance_Lightning);

	/* StatusEffectTypesToResistances */
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Negative_Stun,
	                                            GameplayTags.Attributes_Resistance_Stun);
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Negative_Burning,
	                                            GameplayTags.Attributes_Resistance_Burning);
#pragma endregion

	/* Primary Attributes */
#pragma region Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), "Increases Melee and Ranged Attack Damage");
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"), "Increases Melee and Ranged Weapon Accuracy");
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), "Increases Defense against Physical Attacks");
	GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Agility"), "Increases Evasion from Physical Attacks");
	GameplayTags.Attributes_Primary_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Spirit"),
		"Increases Healing Magic Potency and Improves Resistance to Status Ailments");
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), "Increases Attack Magic Potency and Accuracy");
	GameplayTags.Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vitality"), "Increases Maximum Health");
	GameplayTags.Attributes_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Luck"),
		"Slightly increases Critical Hit Chance and opportunities to evade Status Ailments");
	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Endurance"), "Increases Health Regeneration");
	GameplayTags.Attributes_Primary_Willpower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Willpower"), "Increases Mana Regeneration");
#pragma endregion

	/* Secondary Attributes */
#pragma region Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), "The Maximum Health of the Character/Player");
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), "The Maximum Mana of the Character/Player");
	GameplayTags.Attributes_Secondary_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Attack"), "The Physical Attack Damage of the Character/Player");
	GameplayTags.Attributes_Secondary_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Defense"), "Reduces Damage taken from Physical Attacks");
	GameplayTags.Attributes_Secondary_DodgeChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.DodgeChance"), "Chance to completely Negate Damage from a Physical Attack");
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		"Chance to reduce Damage from a Physical Attack by a certain amount");
	GameplayTags.Attributes_Secondary_MagicalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicalAttack"), "The Magical Attack Damage of the Character/Player");
	GameplayTags.Attributes_Secondary_MagicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicalDefense"), "Reduces Magical Damage taken from Magical Attacks");
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"), "Ignores Enemy Defense");
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		"Chance for additional Bonus Damage to be applied based on the Critical Damage Amount");
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), "The amount of Bonus Damage to apply");
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"), "Reduces the Critical Hit Chance of attacking Enemies");
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		"Slowly restores the Health of the Character/Player per Tick");
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), "Slowly restores the Mana of the Character/Player per Tick");
#pragma endregion

	/* Vital Attributes */
#pragma region Vital Attributes
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"), "Amount of Damage a Character/Player can take before Death");
	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Mana"), "A Resource used in the Casting of Spells and Abilities");
	GameplayTags.Attributes_Vital_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Stamina"),
		"A Resource used for Running, Swimming, Climbing, or other energy-draining activities");
#pragma endregion

	/* Meta Attributes */
#pragma region Meta Attributes
	GameplayTags.Attributes_Meta_IncomingDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingDamage"), "Incoming Damage Meta Attribute");
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"), "Incoming XP Meta Attribute");
#pragma endregion
}
