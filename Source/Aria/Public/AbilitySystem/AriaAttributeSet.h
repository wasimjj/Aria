// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AriaAttributeSet.generated.h"

#pragma region AttributeAccessors
// Macros for Getting, Setting, and Initializing Attributes
// Note: Init sets both the Base Value and Current Value
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
  GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
#pragma endregion

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceAsc = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetAsc = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// This creates an Alias (that accepts any Type <T, FDef...>) for a Function Pointer
template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


/**
 * The UAriaAttributeSet class is a derived class of UAttributeSet. It defines the attributes for a character in the game.
 *
 * This class includes functions for attribute replication, attribute changes, and attribute-specific events.
 * It also provides accessor macros for each attribute.
 *
 * Attributes are grouped into primary and secondary categories.
 *
 * Primary attributes include basic attributes like Strength, Dexterity, and Intelligence,
 * while secondary attributes include derived attributes like MaxHealth, Attack, and Defense.
 *
 * To create attributes, the following steps should be followed:
 * 1. Declare a variable of type FGameplayAttributeData with a unique name.
 * 2. Use the UPROPERTY macro with the ReplicatedUsing parameter to make the attribute replicated and bind
 * it to a corresponding "OnRep" function.
 * 3. Add the ATTRIBUTE_ACCESSORS macro to generate accessor functions for the attribute.
 * 4. Implement the "OnRep" function for the attribute to handle changes in the attribute's value.
 * 5. Register the attribute in the GetLifetimeReplicatedProps function.
 *
 * The UAriaAttributeSet class also includes a TMap to map gameplay tags to attribute delegates.
 * This allows for easy retrieval of attributes based on tags.
 *
 * Most Attributes are Replicable; with some exceptions; such as the Meta Attributes.
 */
UCLASS()
class ARIA_API UAriaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAriaAttributeSet();

	// This is the main function needed to register Variables (attributes) for Replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// This function is triggered before the Attribute is changed.
	// Note: It is not recommended to fire off events or kick off any Gameplay Logic in this function.
	// It is recommended to just use this function for Attribute Clamping.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// This function is triggered after the Attribute has been changed.
	// This function is useful for responding to Gameplay Effects being executed.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// This function is used for when needing to perform some logic directly AFTER an Attribute changes.
	//  Note: This helps fix some issues like the Health and Mana not getting filled up to maximum after a Level Up
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, const float OldValue,
	                                 const float NewValue) override;

	// Tags to Attributes takes a Gameplay Tag for the Key and a Delegate for the Value
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/* Primary (RPG-Style) Attributes */
#pragma region Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Dexterity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Agility);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Spirit, Category = "Primary Attributes")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Spirit);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vitality, Category = "Primary Attributes")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Vitality);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Luck, Category = "Primary Attributes")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Luck);

	// Other Primary Attributes (These can be used for anything; such as controlling Health and Mana Regen, or Stamina Regen)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Endurance, Category = "Primary Attributes|Other")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Endurance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Willpower, Category = "Primary Attributes|Other")
	FGameplayAttributeData Willpower;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Willpower);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;

	UFUNCTION()
	void OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;

	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& OldLuck) const;

	// Other Primary Attributes
	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const;

	UFUNCTION()
	void OnRep_Willpower(const FGameplayAttributeData& OldWillpower) const;
#pragma endregion

	/* Secondary Attributes */
#pragma region Secondary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack, Category = "Secondary Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Attack);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Secondary Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DodgeChance, Category = "Secondary Attributes")
	FGameplayAttributeData DodgeChance;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, DodgeChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalAttack, Category = "Secondary Attributes")
	FGameplayAttributeData MagicalAttack;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, MagicalAttack);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalDefense, Category = "Secondary Attributes")
	FGameplayAttributeData MagicalDefense;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, MagicalDefense);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ManaRegeneration);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;

	UFUNCTION()
	void OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack) const;

	UFUNCTION()
	void OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
#pragma endregion

	/* Vital Attributes */
#pragma region Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, Stamina);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
#pragma endregion

	/* Resistance Attributes */
#pragma region Resistance Attributes

	/* Damage Type Resistances */
#pragma region DamageTypeResistances
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistancePhysical, Category = "Resistance Attributes")
	FGameplayAttributeData ResistancePhysical;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistancePhysical);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceArcane, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceArcane;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceArcane);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistancePoison, Category = "Resistance Attributes")
	FGameplayAttributeData ResistancePoison;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistancePoison);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceEarth, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceEarth;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceEarth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceWind, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceWind;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceWind);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceWater, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceWater;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceWater);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceFire, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceFire;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceFire);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceIce, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceIce;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceIce);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceLightning, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceLightning;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceLightning);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceLight, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceLight;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceLight);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceDark, Category = "Resistance Attributes")
	FGameplayAttributeData ResistanceDark;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, ResistanceDark);

	UFUNCTION()
	void OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const;

	UFUNCTION()
	void OnRep_ResistanceArcane(const FGameplayAttributeData& OldRResistanceArcane) const;

	UFUNCTION()
	void OnRep_ResistancePoison(const FGameplayAttributeData& OldResistancePoison) const;

	UFUNCTION()
	void OnRep_ResistanceEarth(const FGameplayAttributeData& OldResistanceEarth) const;

	UFUNCTION()
	void OnRep_ResistanceWind(const FGameplayAttributeData& OldResistanceWind) const;

	UFUNCTION()
	void OnRep_ResistanceWater(const FGameplayAttributeData& OldResistanceWater) const;

	UFUNCTION()
	void OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const;

	UFUNCTION()
	void OnRep_ResistanceIce(const FGameplayAttributeData& OldResistanceIce) const;

	UFUNCTION()
	void OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const;

	UFUNCTION()
	void OnRep_ResistanceLight(const FGameplayAttributeData& OldResistanceLight) const;

	UFUNCTION()
	void OnRep_ResistanceDark(const FGameplayAttributeData& OldResistanceDark) const;
#pragma endregion

	/* Status Type Resistances */
#pragma region StatusTypeResistances
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistPoison, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistPoison;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistPoison);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistParalysis, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistParalysis;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistParalysis);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistFear, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistFear;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistFear);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistBlindness, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistBlindness;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistBlindness);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistConfusion, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistConfusion;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistConfusion);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistCharm, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistCharm;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistCharm);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistSleep, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistSleep;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistSleep);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistStun, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistStun;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistStun);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistBurning, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistBurning;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistBurning);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistBleeding, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistBleeding;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistBleeding);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistFrozen, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistFrozen;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistFrozen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistPetrification, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistPetrification;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistPetrification);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistStone, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistStone;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistStone);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistCripple, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistCripple;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistCripple);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistRoot, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistRoot;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistRoot);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakness, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakness;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakness);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistSeal, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistSeal;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistSeal);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistExpunge, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistExpunge;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistExpunge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistPureWave, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistPureWave;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistPureWave);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistCurse, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistCurse;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistCurse);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistZombie, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistZombie;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistZombie);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistGrim, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistGrim;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistGrim);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistInstantDeath, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistInstantDeath;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistInstantDeath);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDelevel, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDelevel;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDelevel);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistHealthBurn, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistHealthBurn;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistHealthBurn);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistManaBurn, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistManaBurn;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistManaBurn);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistStaminaBurn, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistStaminaBurn;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistStaminaBurn);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseStrength,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseStrength;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseStrength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseDexterity,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseDexterity;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseDexterity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseResilience,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseResilience;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseResilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseAgility,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseAgility;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseAgility);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseSpirit, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseSpirit;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseSpirit);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseIntelligence,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseIntelligence;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseIntelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseVitality,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseVitality;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseVitality);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseLuck, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseLuck;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseLuck);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseEndurance,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseEndurance;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseEndurance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistDecreaseWillpower,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistDecreaseWillpower;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistDecreaseWillpower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToPhysical, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToPhysical;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToPhysical);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToArcane, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToArcane;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToArcane);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToPoison, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToPoison;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToPoison);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToEarth, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToEarth;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToEarth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToWind, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToWind;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToWind);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToWater, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToWater;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToWater);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToFire, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToFire;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToFire);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToIce, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToIce;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToIce);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToLightning,
		Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToLightning;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToLightning);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToLight, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToLight;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToLight);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StatusResistWeakToDark, Category = "Resistance Attributes")
	FGameplayAttributeData StatusResistWeakToDark;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, StatusResistWeakToDark);

	UFUNCTION()
	void OnRep_StatusResistPoison(const FGameplayAttributeData& OldStatusResistPoison) const;

	UFUNCTION()
	void OnRep_StatusResistParalysis(const FGameplayAttributeData& OldStatusResistParalysis) const;

	UFUNCTION()
	void OnRep_StatusResistFear(const FGameplayAttributeData& OldStatusResistFear) const;

	UFUNCTION()
	void OnRep_StatusResistBlindness(const FGameplayAttributeData& OldStatusResistBlindness) const;

	UFUNCTION()
	void OnRep_StatusResistConfusion(const FGameplayAttributeData& OldStatusResistConfusion) const;

	UFUNCTION()
	void OnRep_StatusResistCharm(const FGameplayAttributeData& OldStatusResistCharm) const;

	UFUNCTION()
	void OnRep_StatusResistSleep(const FGameplayAttributeData& OldStatusResistSleep) const;

	UFUNCTION()
	void OnRep_StatusResistStun(const FGameplayAttributeData& OldStatusResistStun) const;

	UFUNCTION()
	void OnRep_StatusResistBurning(const FGameplayAttributeData& OldStatusResistBurning) const;

	UFUNCTION()
	void OnRep_StatusResistBleeding(const FGameplayAttributeData& OldStatusResistBleeding) const;

	UFUNCTION()
	void OnRep_StatusResistFrozen(const FGameplayAttributeData& OldStatusResistFrozen) const;

	UFUNCTION()
	void OnRep_StatusResistPetrification(const FGameplayAttributeData& OldStatusResistPetrification) const;

	UFUNCTION()
	void OnRep_StatusResistStone(const FGameplayAttributeData& OldStatusResistStone) const;

	UFUNCTION()
	void OnRep_StatusResistCripple(const FGameplayAttributeData& OldStatusResistCripple) const;

	UFUNCTION()
	void OnRep_StatusResistRoot(const FGameplayAttributeData& OldStatusResistRoot) const;

	UFUNCTION()
	void OnRep_StatusResistWeakness(const FGameplayAttributeData& OldStatusResistWeakness) const;

	UFUNCTION()
	void OnRep_StatusResistSeal(const FGameplayAttributeData& OldStatusResistSeal) const;

	UFUNCTION()
	void OnRep_StatusResistExpunge(const FGameplayAttributeData& OldStatusResistExpunge) const;

	UFUNCTION()
	void OnRep_StatusResistPureWave(const FGameplayAttributeData& OldStatusResistPureWave) const;

	UFUNCTION()
	void OnRep_StatusResistCurse(const FGameplayAttributeData& OldStatusResistCurse) const;

	UFUNCTION()
	void OnRep_StatusResistZombie(const FGameplayAttributeData& OldStatusResistZombie) const;

	UFUNCTION()
	void OnRep_StatusResistGrim(const FGameplayAttributeData& OldStatusResistGrim) const;

	UFUNCTION()
	void OnRep_StatusResistInstantDeath(const FGameplayAttributeData& OldStatusResistInstantDeath) const;

	UFUNCTION()
	void OnRep_StatusResistDelevel(const FGameplayAttributeData& OldStatusResistDelevel) const;

	UFUNCTION()
	void OnRep_StatusResistHealthBurn(const FGameplayAttributeData& OldStatusResistHealthBurn) const;

	UFUNCTION()
	void OnRep_StatusResistManaBurn(const FGameplayAttributeData& OldStatusResistManaBurn) const;

	UFUNCTION()
	void OnRep_StatusResistStaminaBurn(const FGameplayAttributeData& OldStatusResistStaminaBurn) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseStrength(const FGameplayAttributeData& OldStatusResistDecreaseStrength) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseDexterity(const FGameplayAttributeData& OldStatusResistDecreaseDexterity) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseResilience(const FGameplayAttributeData& OldStatusResistDecreaseResilience) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseAgility(const FGameplayAttributeData& OldStatusResistDecreaseAgility) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseSpirit(const FGameplayAttributeData& OldStatusResistDecreaseSpirit) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseIntelligence(
		const FGameplayAttributeData& OldStatusResistDecreaseIntelligence) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseVitality(const FGameplayAttributeData& OldStatusResistDecreaseVitality) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseLuck(const FGameplayAttributeData& OldStatusResistDecreaseLuck) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseEndurance(const FGameplayAttributeData& OldStatusResistDecreaseEndurance) const;

	UFUNCTION()
	void OnRep_StatusResistDecreaseWillpower(const FGameplayAttributeData& OldStatusResistDecreaseWillpower) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToPhysical(const FGameplayAttributeData& OldStatusResistWeakToPhysical) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToArcane(const FGameplayAttributeData& OldStatusResistWeakToArcane) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToPoison(const FGameplayAttributeData& OldStatusResistWeakToPoison) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToEarth(const FGameplayAttributeData& OldStatusResistWeakToEarth) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToWind(const FGameplayAttributeData& OldStatusResistWeakToWind) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToWater(const FGameplayAttributeData& OldStatusResistWeakToWater) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToFire(const FGameplayAttributeData& OldStatusResistWeakToFire) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToIce(const FGameplayAttributeData& OldStatusResistWeakToIce) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToLightning(const FGameplayAttributeData& OldStatusResistWeakToLightning) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToLight(const FGameplayAttributeData& OldStatusResistWeakToLight) const;

	UFUNCTION()
	void OnRep_StatusResistWeakToDark(const FGameplayAttributeData& OldStatusResistWeakToDark) const;
#pragma endregion

#pragma endregion

	/* Meta Attributes */
#pragma region Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAriaAttributeSet, IncomingXP);
#pragma endregion

private:
	void HandleIncomingDamage(const FEffectProperties& EffectProps);
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	static void ShowDamageFloatingText(const FEffectProperties& EffectProps, const float Damage, const bool bDodgedHit,
	                                   const bool bBlockedHit, const bool bCriticalHit);

	void HandleStatusEffect(const FEffectProperties& EffectProps) const;

	void HandleIncomingXP(const FEffectProperties& EffectProps);
	static void SendXPEvent(const FEffectProperties& EffectProps);

	// Used when determining whether or not we need to 'Top Off' our Fluids (such as after a Level Up)
	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};
