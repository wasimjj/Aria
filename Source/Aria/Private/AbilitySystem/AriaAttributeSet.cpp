// Copyright Savoie Interactive Studios


#include "AbilitySystem/AriaAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "AriaGameplayTags.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/XPInterface.h"
#include "Player/AriaPlayerController.h"

UAriaAttributeSet::UAriaAttributeSet()
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	// Meta Incoming Damage Attribute : This is not replicable
	TagsToAttributes.Add(GameplayTags.Attributes_Meta_IncomingDamage, GetIncomingDamageAttribute);

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Agility, GetAgilityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Spirit, GetSpiritAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vitality, GetVitalityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Luck, GetLuckAttribute);
	// Other Primary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Endurance, GetEnduranceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Willpower, GetWillpowerAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Attack, GetAttackAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Defense, GetDefenseAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_DodgeChance, GetDodgeChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MagicalAttack, GetMagicalAttackAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MagicalDefense, GetMagicalDefenseAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);

	/* Resistance Attributes */
#pragma region ResistanceAttributes

	/* Damage Type Resistances */
#pragma region DamageTypeResistances
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetResistancePhysicalAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetResistanceArcaneAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetResistanceFireAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetResistanceLightningAttribute);
#pragma endregion

	/* Status Type Resistances */
#pragma region StatusTypeResistances
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Stun, GetStatusResistStunAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Burning, GetStatusResistBurningAttribute);
#pragma endregion

#pragma endregion
}

// The main function used for registering Variables for replication.
void UAriaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// This takes care of registering a Variable for replication.
	//  (DOREPLIFETIME_CONDITION_NOTIFY takes the class name, the replicated attribute name, the condition needed
	//  for the Variable to replicate, and how often (when) to Replicate).
	// REPNOTIFY_Always means that if the value is set on the Server, always replicate it on the Client.
	//	 - REPNOTIFY_Always is useful for when you want to respond to the value change regardless of if the value is different or not.
	// REPNOTIFY_OnChanged (Default) means that if the value is set on the Server, and that value has not changed, then do not replicate it to the Client.

	/* Primary (RPG-Style) Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Spirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Luck, COND_None, REPNOTIFY_Always);
	// Other Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Willpower, COND_None, REPNOTIFY_Always);

	/* Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, DodgeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, MagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, MagicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	/* Vital Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	/* Resistance Attributes */
#pragma region ResistanceAttributes

	/* Damage Type Resistances */
#pragma region DamageTypeResistances
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistancePoison, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceEarth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceWind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceWater, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceIce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceLight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, ResistanceDark, COND_None, REPNOTIFY_Always);
#pragma endregion

	/* Status Type Resistances */
#pragma region StatusTypeResistances
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistPoison, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistParalysis, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistFear, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistBlindness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistConfusion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistCharm, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistSleep, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistStun, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistBurning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistBleeding, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistFrozen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistPetrification, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistStone, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistCripple, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistRoot, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistSeal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistExpunge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistPureWave, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistCurse, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistZombie, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistGrim, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistInstantDeath, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDelevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistHealthBurn, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistManaBurn, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistStaminaBurn, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseStrength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseDexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseResilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseAgility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseSpirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseIntelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseVitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseLuck, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseEndurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistDecreaseWillpower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToPhysical, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToPoison, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToEarth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToWind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToWater, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToIce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToLight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAriaAttributeSet, StatusResistWeakToDark, COND_None, REPNOTIFY_Always);
#pragma endregion

#pragma endregion
}

// When an Attribute Replicates via the "OnRep_<attributename>" function, it will now be equal to its new replicated value.
// But we will still know the old value thanks to the Old<attributename> Parameter reference being passed in.
// Note: This OldValue param is an inherent quality of Rep Notifies;
// (i.e. Adding that Param automatically gets the old value and stores it in there).

/* On Rep - Primary Attributes */
#pragma region OnRep_PrimaryAttributes
void UAriaAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Strength, OldStrength);
}

void UAriaAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Dexterity, OldDexterity);
}

void UAriaAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Resilience, OldResilience);
}

void UAriaAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Agility, OldAgility);
}

void UAriaAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Spirit, OldSpirit);
}

void UAriaAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Intelligence, OldIntelligence);
}

void UAriaAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Vitality, OldVitality);
}

void UAriaAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Luck, OldLuck);
}

void UAriaAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Endurance, OldEndurance);
}

void UAriaAttributeSet::OnRep_Willpower(const FGameplayAttributeData& OldWillpower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Willpower, OldWillpower);
}
#pragma endregion

/* On Rep - Secondary Attributes */
#pragma region OnRep_SecondaryAttributes
void UAriaAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, MaxHealth, OldMaxHealth);
}

void UAriaAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, MaxMana, OldMaxMana);
}

void UAriaAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Attack, OldAttack);
}

void UAriaAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Defense, OldDefense);
}

void UAriaAttributeSet::OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, DodgeChance, OldDodgeChance);
}

void UAriaAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, BlockChance, OldBlockChance);
}

void UAriaAttributeSet::OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, MagicalAttack, OldMagicalAttack);
}

void UAriaAttributeSet::OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, MagicalDefense, OldMagicalDefense);
}

void UAriaAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAriaAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAriaAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAriaAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAriaAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAriaAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ManaRegeneration, OldManaRegeneration);
}
#pragma endregion

/* On Rep - Vital Attributes */
#pragma region OnRep_VitalAttributes
void UAriaAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// This takes care of Informing the Ability System that we are replicating a value.
	// Now the Ability System can register that change and keep track of the old value in case anything needs to be rolled back.
	//  (GAMEPLAYATTRIBUTE_REPNOTIFY takes the class name, the replicated attribute name, and even the old attribute name).

	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Health, OldHealth);
}

void UAriaAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Mana, OldMana);
}

void UAriaAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, Stamina, OldStamina);
}
#pragma endregion

/* On Rep - Resistance Attributes */
#pragma region OnRep_ResistanceAttributes

/* Damage Type Resistances */
#pragma region DamageTypeResistances
void UAriaAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistancePhysical, OldResistancePhysical);
}

void UAriaAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldRResistanceArcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldRResistanceArcane);
}

void UAriaAttributeSet::OnRep_ResistancePoison(const FGameplayAttributeData& OldResistancePoison) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistancePoison, OldResistancePoison);
}

void UAriaAttributeSet::OnRep_ResistanceEarth(const FGameplayAttributeData& OldResistanceEarth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceEarth, OldResistanceEarth);
}

void UAriaAttributeSet::OnRep_ResistanceWind(const FGameplayAttributeData& OldResistanceWind) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceWind, OldResistanceWind);
}

void UAriaAttributeSet::OnRep_ResistanceWater(const FGameplayAttributeData& OldResistanceWater) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceWater, OldResistanceWater);
}

void UAriaAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceFire, OldResistanceFire);
}

void UAriaAttributeSet::OnRep_ResistanceIce(const FGameplayAttributeData& OldResistanceIce) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceIce, OldResistanceIce);
}

void UAriaAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceLightning, OldResistanceLightning);
}

void UAriaAttributeSet::OnRep_ResistanceLight(const FGameplayAttributeData& OldResistanceLight) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceLight, OldResistanceLight);
}

void UAriaAttributeSet::OnRep_ResistanceDark(const FGameplayAttributeData& OldResistanceDark) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceDark, OldResistanceDark);
}
#pragma endregion

/* Status Type Resistances */
#pragma region StatusTypeResistances
void UAriaAttributeSet::OnRep_StatusResistPoison(const FGameplayAttributeData& OldStatusResistPoison) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistPoison);
}

void UAriaAttributeSet::OnRep_StatusResistParalysis(const FGameplayAttributeData& OldStatusResistParalysis) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistParalysis);
}

void UAriaAttributeSet::OnRep_StatusResistFear(const FGameplayAttributeData& OldStatusResistFear) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistFear);
}

void UAriaAttributeSet::OnRep_StatusResistBlindness(const FGameplayAttributeData& OldStatusResistBlindness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistBlindness);
}

void UAriaAttributeSet::OnRep_StatusResistConfusion(const FGameplayAttributeData& OldStatusResistConfusion) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistConfusion);
}

void UAriaAttributeSet::OnRep_StatusResistCharm(const FGameplayAttributeData& OldStatusResistCharm) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistCharm);
}

void UAriaAttributeSet::OnRep_StatusResistSleep(const FGameplayAttributeData& OldStatusResistSleep) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistSleep);
}

void UAriaAttributeSet::OnRep_StatusResistStun(const FGameplayAttributeData& OldStatusResistStun) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistStun);
}

void UAriaAttributeSet::OnRep_StatusResistBurning(const FGameplayAttributeData& OldStatusResistBurning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistBurning);
}

void UAriaAttributeSet::OnRep_StatusResistBleeding(const FGameplayAttributeData& OldStatusResistBleeding) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistBleeding);
}

void UAriaAttributeSet::OnRep_StatusResistFrozen(const FGameplayAttributeData& OldStatusResistFrozen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistFrozen);
}

void UAriaAttributeSet::OnRep_StatusResistPetrification(
	const FGameplayAttributeData& OldStatusResistPetrification) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistPetrification);
}

void UAriaAttributeSet::OnRep_StatusResistStone(const FGameplayAttributeData& OldStatusResistStone) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistStone);
}

void UAriaAttributeSet::OnRep_StatusResistCripple(const FGameplayAttributeData& OldStatusResistCripple) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistCripple);
}

void UAriaAttributeSet::OnRep_StatusResistRoot(const FGameplayAttributeData& OldStatusResistRoot) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistRoot);
}

void UAriaAttributeSet::OnRep_StatusResistWeakness(const FGameplayAttributeData& OldStatusResistWeakness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakness);
}

void UAriaAttributeSet::OnRep_StatusResistSeal(const FGameplayAttributeData& OldStatusResistSeal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistSeal);
}

void UAriaAttributeSet::OnRep_StatusResistExpunge(const FGameplayAttributeData& OldStatusResistExpunge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistExpunge);
}

void UAriaAttributeSet::OnRep_StatusResistPureWave(const FGameplayAttributeData& OldStatusResistPureWave) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistPureWave);
}

void UAriaAttributeSet::OnRep_StatusResistCurse(const FGameplayAttributeData& OldStatusResistCurse) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistCurse);
}

void UAriaAttributeSet::OnRep_StatusResistZombie(const FGameplayAttributeData& OldStatusResistZombie) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistZombie);
}

void UAriaAttributeSet::OnRep_StatusResistGrim(const FGameplayAttributeData& OldStatusResistGrim) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistGrim);
}

void UAriaAttributeSet::OnRep_StatusResistInstantDeath(const FGameplayAttributeData& OldStatusResistInstantDeath) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistInstantDeath);
}

void UAriaAttributeSet::OnRep_StatusResistDelevel(const FGameplayAttributeData& OldStatusResistDelevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDelevel);
}

void UAriaAttributeSet::OnRep_StatusResistHealthBurn(const FGameplayAttributeData& OldStatusResistHealthBurn) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistHealthBurn);
}

void UAriaAttributeSet::OnRep_StatusResistManaBurn(const FGameplayAttributeData& OldStatusResistManaBurn) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistManaBurn);
}

void UAriaAttributeSet::OnRep_StatusResistStaminaBurn(const FGameplayAttributeData& OldStatusResistStaminaBurn) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistStaminaBurn);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseStrength(
	const FGameplayAttributeData& OldStatusResistDecreaseStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseStrength);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseDexterity(
	const FGameplayAttributeData& OldStatusResistDecreaseDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseDexterity);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseResilience(
	const FGameplayAttributeData& OldStatusResistDecreaseResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseResilience);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseAgility(
	const FGameplayAttributeData& OldStatusResistDecreaseAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseAgility);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseSpirit(
	const FGameplayAttributeData& OldStatusResistDecreaseSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseSpirit);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseIntelligence(
	const FGameplayAttributeData& OldStatusResistDecreaseIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseIntelligence);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseVitality(
	const FGameplayAttributeData& OldStatusResistDecreaseVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseVitality);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseLuck(const FGameplayAttributeData& OldStatusResistDecreaseLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseLuck);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseEndurance(
	const FGameplayAttributeData& OldStatusResistDecreaseEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseEndurance);
}

void UAriaAttributeSet::OnRep_StatusResistDecreaseWillpower(
	const FGameplayAttributeData& OldStatusResistDecreaseWillpower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistDecreaseWillpower);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToPhysical(
	const FGameplayAttributeData& OldStatusResistWeakToPhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToPhysical);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToArcane(const FGameplayAttributeData& OldStatusResistWeakToArcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToArcane);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToPoison(const FGameplayAttributeData& OldStatusResistWeakToPoison) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToPoison);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToEarth(const FGameplayAttributeData& OldStatusResistWeakToEarth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToEarth);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToWind(const FGameplayAttributeData& OldStatusResistWeakToWind) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToWind);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToWater(const FGameplayAttributeData& OldStatusResistWeakToWater) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToWater);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToFire(const FGameplayAttributeData& OldStatusResistWeakToFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToFire);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToIce(const FGameplayAttributeData& OldStatusResistWeakToIce) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToIce);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToLightning(
	const FGameplayAttributeData& OldStatusResistWeakToLightning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToLightning);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToLight(const FGameplayAttributeData& OldStatusResistWeakToLight) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToLight);
}

void UAriaAttributeSet::OnRep_StatusResistWeakToDark(const FGameplayAttributeData& OldStatusResistWeakToDark) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAriaAttributeSet, ResistanceArcane, OldStatusResistWeakToDark);
}
#pragma endregion

#pragma endregion

void UAriaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		//UE_LOG(LogAria, Warning, TEXT("Health: %f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		//UE_LOG(LogAria, Warning, TEXT("Mana: %f"), NewValue);
	}
}

void UAriaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Note: In order for Data to have access to the other Intellisense properties, #include "GameplayEffectExtension.h" MUST be included!

	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProps;
	SetEffectProperties(Data, EffectProps);

	// Perform a Death Check; All of the Logic below should be ignored if the Target is Dead
	if (EffectProps.TargetAvatarActor->Implements<UCombatInterface>() &&
		ICombatInterface::Execute_IsDead(EffectProps.TargetAvatarActor))
		return;

	// Handles Clamping of the final Attribute result
	// Fixes issues where being Damaged wouldn't reduce Health.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	/* Meta Attribute "Incoming Damage" Calculation Logic (Includes Applying Status Effects) */
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(EffectProps);
	}

	/* Meta Attribute "Incoming XP" Calculation Logic */
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(EffectProps);
	}
}

/* Handle Incoming Damage (Including call to 'HandleStatusEffects') */
#pragma region HandleIncomingDamage
void UAriaAttributeSet::HandleIncomingDamage(const FEffectProperties& EffectProps)
{
	// Consume Incoming Damage and store into a new local variable
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		// bFatal determines if the Character should be destroyed or perform other actions (such as a hit reaction)
		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			// Check if the Character or Object implements the Combat Interface
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatarActor))
			{
				CombatInterface->Die(UAriaAbilitySystemLibrary::GetDeathImpulse(EffectProps.EffectContextHandle));
			}
			// Sends an XP Reward (where applicable) when that Target Dies.
			SendXPEvent(EffectProps);
		}
		else
		{
			// Perform a Knockback (when applicable)
			const FVector& KnockbackForce = UAriaAbilitySystemLibrary::GetKnockbackForce(
				EffectProps.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				EffectProps.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}

			FGameplayTagContainer TagContainer;
			
			// Check whether being Shocked; if so completely avoid the Hit React
			if (EffectProps.TargetAvatarActor->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_GetIsBeingShocked(EffectProps.TargetAvatarActor))
			{
				// Adds the Hit React Tag to the Tag Container used for ability activation
				TagContainer.AddTag(FAriaGameplayTags::Get().Effects_HitReact);
			}

			// Activates any Ability(ies) matching that specific Ability Tag
			EffectProps.TargetAsc->TryActivateAbilitiesByTag(TagContainer);
		}
	}

	// Set the additional combat properties (for Dodged, Blocked, Critical Hit, etc.)
	const bool bDodge = UAriaAbilitySystemLibrary::IsDodgedHit(EffectProps.EffectContextHandle);
	const bool bBlock = UAriaAbilitySystemLibrary::IsBlockedHit(EffectProps.EffectContextHandle);
	const bool bCriticalHit = UAriaAbilitySystemLibrary::IsCriticalHit(EffectProps.EffectContextHandle);

	/* Status Effect Logic */
	if (UAriaAbilitySystemLibrary::IsStatusEffectApplied(EffectProps.EffectContextHandle))
	{
		HandleStatusEffect(EffectProps);
	}

	ShowDamageFloatingText(EffectProps, LocalIncomingDamage, bDodge, bBlock, bCriticalHit);
}

void UAriaAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	// Source = The Causer of the Effect, Target = the Target of the Effect (Target is also the owner of this Ability System)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();

	/* Get Source Info */
#pragma region Get Source Info
	Props.SourceAsc = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceAsc) && Props.SourceAsc->AbilityActorInfo.IsValid() && Props.SourceAsc->AbilityActorInfo->
		AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceAsc->AbilityActorInfo->AvatarActor.Get();
		// First; attempt to get the Controller from the AbilityActorInfo 
		Props.SourceController = Props.SourceAsc->AbilityActorInfo->PlayerController.Get();
		// If AbilityActorInfo contains a nullptr for the Source Controller, then try to get it through the Source Avatar Actor instead
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			// Attempt to get the Source Controller from the Source Pawn directly
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				// If the cast is successful then we can get the Source Controller from the Source Pawn
				Props.SourceController = Pawn->GetController();
			}
		}
		// Attempt to get the Source Character
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
#pragma endregion

	/* Get Target Info */
#pragma region Get Target Info
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
#pragma endregion
}

void UAriaAttributeSet::ShowDamageFloatingText(const FEffectProperties& EffectProps, const float Damage,
                                               const bool bDodgedHit, const bool bBlockedHit, const bool bCriticalHit)
{
	if (EffectProps.SourceCharacter != EffectProps.TargetCharacter)
	{
		if (AAriaPlayerController* PC = Cast<AAriaPlayerController>(EffectProps.SourceCharacter->Controller))
		{
			PC->ShowDamageFloatingText(Damage, EffectProps.TargetCharacter, bDodgedHit, bBlockedHit, bCriticalHit);
		}

		if (AAriaPlayerController* PC = Cast<AAriaPlayerController>(EffectProps.TargetCharacter->Controller))
		{
			PC->ShowDamageFloatingText(Damage, EffectProps.TargetCharacter, bDodgedHit, bBlockedHit, bCriticalHit);
		}
	}
}
#pragma endregion

/* Handle Status Effects(s) */
#pragma region HandleStatusEffect
void UAriaAttributeSet::HandleStatusEffect(const FEffectProperties& EffectProps) const
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	// This is the Effect Context that caused the Status Effect 
	FGameplayEffectContextHandle EffectContext = EffectProps.SourceAsc->MakeEffectContext();
	EffectContext.AddSourceObject(EffectProps.SourceAvatarActor);

	// Configure the Status Effect Gameplay Effect (GE)
#pragma region StatusEffectSetup
	// Collect all of the information needed for the Status Effect Gameplay Effect
	const FGameplayTag StatusEffectType = UAriaAbilitySystemLibrary::GetStatusEffectType(
		EffectProps.EffectContextHandle);
	const float StatusEffectDuration = UAriaAbilitySystemLibrary::GetStatusEffectDuration(
		EffectProps.EffectContextHandle);
	const float StatusEffectFrequency = UAriaAbilitySystemLibrary::GetStatusEffectFrequency(
		EffectProps.EffectContextHandle);
	const FGameplayTag DamagedAttributeType = UAriaAbilitySystemLibrary::GetDamagedAttributeType(
		EffectProps.EffectContextHandle);
	const float DamagedAttributeAmount = UAriaAbilitySystemLibrary::GetDamagedAttributeAmount(
		EffectProps.EffectContextHandle);

	// Dynamically create the Status Effect Gameplay Effect (GE)
	
	
	if(!StatusEffectType.GetTagName().ToString().Contains(TEXT("None")))
	{
		const FString StatusEffectName = FString::Printf(
		TEXT("Dynamic Status Effect (GE): %s"), *StatusEffectType.GetTagName().ToString());
		GLog->Log(FString::Printf(TEXT("StatusEffect:::::: %s"), *StatusEffectName));

		if(UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(StatusEffectName)))
		{
				// Configures the Duration Policy
		Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		Effect->DurationMagnitude = FScalableFloat(StatusEffectDuration);
		Effect->Period = StatusEffectFrequency;
		Effect->bExecutePeriodicEffectOnApplication = false; // TODO: Change to use an AbilityModifier property.

		// Configures any Tags (i.e. Granting Status Effect Tags)
#pragma region GrantTags
		FInheritedTagContainer TagContainer = FInheritedTagContainer();
		UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
		TagContainer.Added.AddTag(StatusEffectType);
	
		// Handle the Stun Status Effect
		//  Blocks all Player Input
		if (StatusEffectType.MatchesTagExact(GameplayTags.StatusEffect_Negative_Stun))
		{
			TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
			TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
			TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
			TagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		}
	
		Component.SetAndApplyTargetTagChanges(TagContainer);
#pragma endregion
	
		// Configures Effect Stacking 
		Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
		Effect->StackLimitCount = 1;

		// Configures any Modifiers
		int32 ModIndex = Effect->Modifiers.Num();

		// Damaged Attribute Modifier
		if (DamagedAttributeType.IsValid() && DamagedAttributeAmount > 0.f)
		{
			Effect->Modifiers.Add(FGameplayModifierInfo());
			FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[ModIndex];
			ModifierInfo.Attribute = UAriaAbilitySystemLibrary::GetAttributeByTag(TagsToAttributes, DamagedAttributeType);
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FScalableFloat(DamagedAttributeAmount);
		
			// Note: To add additional Modifiers; just ++Index and repeat the above for Adding another Modifier
		}

		/*// Note: All Status Effects have just a default level so their settings don't really change
		//  TODO: Make it where all Status Effects can have different Levels and Modifiers via Curve Tables
			if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
			{
				FAriaGameplayEffectContext* AriaContext = static_cast<FAriaGameplayEffectContext*>(EffectContext.Get());
				//const TSharedPtr<FGameplayTag> StatEffectType = MakeShareable(new FGameplayTag(StatusEffectType));
				AriaContext->SetStatusEffectType(StatusEffectType);
	#pragma endregion

				// Apply the Status Effect Gameplay Effect (GE)
				EffectProps.TargetAsc->ApplyGameplayEffectSpecToSelf(*MutableSpec);
			}*/
		}

	
	}
}

#pragma endregion

/* Handle Incoming XP */
#pragma region HandleIncomingXP
void UAriaAttributeSet::HandleIncomingXP(const FEffectProperties& EffectProps)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	if (LocalIncomingXP > 0.f)
	{
		//UE_LOG(LogAria, Log, TEXT("Get Incoming XP: %f"), LocalIncomingXP);

		// Add the XP Amount received to the Player Character
		//  Note: SourceCharacter is the Owner; GA_ListenForEvents applies GE_EventBasedEffect; adding to IncomingXP
		if (EffectProps.SourceCharacter->Implements<UXPInterface>() &&
			EffectProps.SourceCharacter->Implements<UCombatInterface>())
		{
			// Store the Player's Current Level and Current XP
			const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProps.SourceCharacter);
			const int32 CurrentXP = IXPInterface::Execute_GetXPTotal(EffectProps.SourceCharacter);

			// Determines if the Player can Level Up based on the Player's new Total XP (Current XP + additional Incoming XP)
			const int32 NewLevel = IXPInterface::Execute_FindLevelForXPTotal(
				EffectProps.SourceCharacter, CurrentXP + LocalIncomingXP);
			// Determines how many times the Level Up can occur
			const int32 NumLevelUps = NewLevel - CurrentLevel;

			// Perform Level Up Steps
			if (NumLevelUps > 0)
			{
				// Level Up the Player Character; Adding the Number of Levels gained to the Player's current Level
				IXPInterface::Execute_AddToLevel(EffectProps.SourceCharacter, NumLevelUps);

				// Get the Attribute and Ability Point rewards from AriaPlayerState > LevelUpInfo
				int32 AttributePointsReward = 0;
				int32 AbilityPointsReward = 0;

				// Also works when leveling up Multiple Times in a row off of a single XP Reward
				for (int32 i = 0; i < NumLevelUps; ++i)
				{
					AttributePointsReward += IXPInterface::Execute_GetAttributePointsReward(EffectProps.SourceCharacter, CurrentLevel + i);
					AbilityPointsReward += IXPInterface::Execute_GetAbilityPointsReward(EffectProps.SourceCharacter, CurrentLevel + i);
				}
				
				// Add the Attribute and Ability Point rewards to the Players current Attribute and Ability totals
				IXPInterface::Execute_AddToAttributePoints(EffectProps.SourceCharacter, AttributePointsReward);
				IXPInterface::Execute_AddToAbilityPoints(EffectProps.SourceCharacter, AbilityPointsReward);

				// Fill up the Player's Health and Mana to Full
				bTopOffHealth = true;
				bTopOffMana = true;

				// Currently needed for handling the LevelUp VFX and SFX functions.
				IXPInterface::Execute_LevelUp(EffectProps.SourceCharacter);
			}

			// Adds the additional Incoming XP amount to the Player's current XP Total
			IXPInterface::Execute_AddXPToXPTotal(EffectProps.SourceCharacter, LocalIncomingXP);
		}
	}
}

void UAriaAttributeSet::SendXPEvent(const FEffectProperties& EffectProps)
{
	if (EffectProps.TargetCharacter->Implements<UCombatInterface>())
	{
		// Get the Target Data; including the XPReward 
		const ECharacterClass TargetCharacterClass = ICombatInterface::Execute_GetCharacterClass(
			EffectProps.TargetCharacter);
		const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProps.TargetCharacter);
		const int32 XPReward = UAriaAbilitySystemLibrary::GetXPRewardForClassAndLevel(
			EffectProps.TargetCharacter, TargetCharacterClass, TargetLevel);

		// Send the Event to the Source Character (the Attacker who defeated the Enemy)
		const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.SourceCharacter,
		                                                         GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}
#pragma endregion

void UAriaAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, const float OldValue,
                                            const float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() & bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() & bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}
