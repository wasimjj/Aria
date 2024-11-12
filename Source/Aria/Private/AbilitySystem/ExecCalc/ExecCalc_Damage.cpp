// Copyright Savoie Interactive Studios


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AriaAbilityTypes.h"
#include "AriaGameplayTags.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

// Here we create (declare and define) Attribute Capture Definitions
// This is a local struct only available to this ExecCalc_Damage class
// This will be instantiated once, and each time it gets accessed, this will be the same instance (much like a singleton)
struct FAriaDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)

	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DodgeChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)

	/* Damage Type Resistances */
#pragma region DamageTypeResistances
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePoison)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceEarth)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceWind)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceWater)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceIce)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLight)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceDark)
#pragma endregion

	/* Status Type Resistances */
#pragma region StatusTypeResistances
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistPoison)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistParalysis)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistFear)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistBlindness)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistConfusion)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistCharm)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistSleep)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistStun)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistBurning)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistBleeding)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistFrozen)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistPetrification)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistStone)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistCripple)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistRoot)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakness)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistSeal)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistExpunge)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistPureWave)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistCurse)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistZombie)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistGrim)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistInstantDeath)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDelevel)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistHealthBurn)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistManaBurn)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistStaminaBurn)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseStrength)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseDexterity)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseResilience)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseAgility)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseSpirit)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseIntelligence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseVitality)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseLuck)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseEndurance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistDecreaseWillpower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToPhysical)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToArcane)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToPoison)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToEarth)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToWind)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToWater)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToFire)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToIce)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToLightning)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToLight)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusResistWeakToDark)
#pragma endregion

	FAriaDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, CriticalHitChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, CriticalHitDamage, Source, false)

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, Defense, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, DodgeChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, CriticalHitResistance, Target, false)

		/* Damage Type Resistances */
#pragma region DamageTypeResistances
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistancePhysical, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceArcane, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistancePoison, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceEarth, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceWind, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceWater, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceFire, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceIce, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceLightning, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceLight, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, ResistanceDark, Target, false)
#pragma endregion

		/* Status Type Resistances */
#pragma region StatusTypeResistances
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistPoison, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistParalysis, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistFear, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistBlindness, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistConfusion, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistCharm, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistSleep, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistStun, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistBurning, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistBleeding, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistFrozen, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistPetrification, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistStone, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistCripple, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistRoot, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakness, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistSeal, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistExpunge, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistPureWave, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistCurse, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistZombie, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistGrim, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistInstantDeath, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDelevel, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistHealthBurn, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistManaBurn, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistStaminaBurn, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseStrength, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseDexterity, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseResilience, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseAgility, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseSpirit, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseIntelligence, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseVitality, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseLuck, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseEndurance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistDecreaseWillpower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToPhysical, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToArcane, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToPoison, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToEarth, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToWind, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToWater, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToFire, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToIce, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToLightning, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToLight, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAriaAttributeSet, StatusResistWeakToDark, Target, false)
#pragma endregion
	}
};

// This static function returns the single (static) instance of the AriaDamageStatics struct object 
static const FAriaDamageStatics& DamageStatics()
{
	// This creates a static AriaDamageStatics struct object
	//  Every time the DamageStatics function gets called, we will use the same DStatics object
	//  DStatics doesn't go away when the function ends
	static FAriaDamageStatics DStatics;

	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Stores the Attributes to capture into the RelevantAttributesToCapture list
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().DodgeChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	/* Damage Type Resistances */
#pragma region DamageTypeResistances
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePhysicalDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceArcaneDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePoisonDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceEarthDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceWindDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceWaterDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceFireDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceIceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightningDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceDarkDef);
#pragma endregion

	/* Status Type Resistances */
#pragma region StatusTypeResistances
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistPoisonDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistParalysisDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistFearDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistBlindnessDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistConfusionDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistCharmDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistSleepDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistStunDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistBurningDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistBleedingDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistFrozenDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistPetrificationDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistStoneDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistCrippleDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistRootDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeaknessDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistSealDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistExpungeDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistPureWaveDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistCurseDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistZombieDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistGrimDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistInstantDeathDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDelevelDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistHealthBurnDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistManaBurnDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistStaminaBurnDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseStrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseDexterityDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseResilienceDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseAgilityDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseSpiritDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseIntelligenceDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseVitalityDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseLuckDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseEnduranceDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistDecreaseWillpowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToPhysicalDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToArcaneDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToPoisonDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToEarthDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToWindDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToWaterDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToFireDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToIceDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToLightningDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToLightDef);
	RelevantAttributesToCapture.Add(DamageStatics().StatusResistWeakToDarkDef);
#pragma endregion
}

void UExecCalc_Damage::ApplyDamage(
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs,
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const UCharacterClassInfo* CharacterClassInfo, int32 SourceCharacterLevel,
	int32 TargetCharacterLevel, const FGameplayEffectSpec& Spec,
	FGameplayEffectContextHandle EffectContextHandle,
	FAggregatorEvaluateParameters EvaluationParameters, float& Damage)
{
	Damage = 0.f;
	// Get the Damage amount from each Damage Type (includes the Basic Damage)
	for (const auto& Pair : FAriaGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag DamageResistanceType = Pair.Value;

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType, false);

		/* == DAMAGE RESISTANCE == */
#pragma region DamageResistance
		// Ensure the Resistance Tag exists in TagsToCaptureDefs; if so assign it to 'CaptureDef'
		checkf(InTagsToCaptureDefs.Contains(DamageResistanceType),
		       TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"),
		       *DamageResistanceType.ToString());
		const FGameplayEffectAttributeCaptureDefinition ResistanceCaptureDef = InTagsToCaptureDefs[
			DamageResistanceType];

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceCaptureDef, EvaluationParameters,
		                                                           Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		// Calculate the Damage minus any Damage Resistance
		//   Example: If Resistance = 10, then 100 - 90, 90 over 100 is .9
		//		       Multiply the Damage Type Value by .9 and set to result effectively reducing the Damage by 10 % 
		// *= We are going to set it to itself times something
		DamageTypeValue *= (100.f - Resistance) / 100.f;

		// Result
		Damage += DamageTypeValue;
#pragma endregion
	}

	/* == DODGE CHANCE == */
#pragma region DodgeChance
	// Capture Dodge Chance on Target, and determine if there was a successful Dodge
	float TargetDodgeChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DodgeChanceDef, EvaluationParameters,
	                                                           TargetDodgeChance);
	TargetDodgeChance = FMath::Max<float>(TargetDodgeChance, 0.f);

	// Check for a Successful Dodge
	if (FMath::RandRange(1, 100) < TargetDodgeChance)
	{
		// A successful Dodge occured, negate all Damage
		Damage = 0;

		UAriaAbilitySystemLibrary::SetIsDodgedHit(EffectContextHandle, true);
	}
#pragma endregion
	else
	{
		// No Dodge occurred... Target now has chance to block the incoming attack

		/* == BLOCK CHANCE == */
#pragma region BlockChance
		// Capture Block Chance on Target, and determine if there was a successful Block
		float TargetBlockChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters,
		                                                           TargetBlockChance);
		TargetBlockChance = FMath::Max<float>(TargetDodgeChance, 0.f);

		// Check for a Successful Block
		if (FMath::RandRange(1, 100) < TargetBlockChance)
		{
			// A successful Block occured, reduce the damage by a percentage (ex: 1/2)
			Damage = Damage / 2.f;

			UAriaAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, true);
		}
#pragma endregion
		else
		{
			// No Block occurred... Calculate Regular Damage (Target is now also susceptible to a Critical Hit)

			/* == DEFENSE FROM ATTACK == */
#pragma region Defence
			// Capture the Target's (Defender) Defense 
			float TargetDefense = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters,
			                                                           TargetDefense);
			TargetDefense = FMath::Max<float>(TargetDefense, 0.f);

			// Capture the Source's (Attacker) Armor Penetration 
			// Note: Armor Penetration ignores a Percentage of the Target's Defense
			float SourceArmorPenetration = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,
			                                                           EvaluationParameters, SourceArmorPenetration);
			SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

			// Get the Armor Penetration Coefficient
			const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
				FName("ArmorPenetration"), FString());
			const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCharacterLevel);

			// This is the amount of Defense left over after any applied reductions
			//  Note: SourceArmorPenetration * ArmorPenetrationCoefficient (ex: 0.25f) means it takes 4 ArmorPenetration points to ignore 1% of the Target's Defense.
			const float EffectiveDefense = TargetDefense * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient)
				/ 100.f;

			// Get the Effective Defense Coefficient
			const FRealCurve* EffectiveDefenseCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
				FName("EffectiveDefense"), FString());
			const float EffectiveDefenseCoefficient = EffectiveDefenseCurve->Eval(TargetCharacterLevel);

			// Reduce the amount of Damage based on the Target's Defense
			//  EffectiveDefense ignores a percentage of incoming damage
			//   Note: EffectiveDefense * EffectiveDefenseCoefficient (ex: 0.333f) means it takes 3 points of Defense to ignore 1% of Damage 
			Damage *= (100 - EffectiveDefense * EffectiveDefenseCoefficient) / 100.f;
#pragma endregion

			/* == CRITICAL HIT == */
#pragma region CriticalHit
			// Capture the Source's (Attacker) Critical Hit Chance 
			float SourceCriticalHitChance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,
			                                                           EvaluationParameters, SourceCriticalHitChance);
			SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

			// Capture the Source's (Attacker) Critical Hit Damage 
			float SourceCriticalHitDamage = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,
			                                                           EvaluationParameters, SourceCriticalHitDamage);
			SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

			// Capture the Target's (Defender) Critical Hit Resistance
			float TargetCriticalHitResistance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,
			                                                           EvaluationParameters,
			                                                           TargetCriticalHitResistance);
			TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

			// Get the Critical Hit Resistance Coefficient
			const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
				FName("CriticalHitResistance"), FString());
			const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCharacterLevel);

			// If a successful Critical Hit occured, double the damage and add a critical damage bonus (from SourceCriticalHitDamage) 
			//  Note: Critical Hit Resistance should reduce the Attacker's Critical Hit Chance by a certain percentage
			const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance *
				CriticalHitResistanceCoefficient;
			const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
			UAriaAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

			// Double the Damage plus apply additional Critical Hit Damage bonus
			Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
#pragma endregion
		}
	}
}

void UExecCalc_Damage::ApplyStatusEffects(
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs,
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec,
	const FAggregatorEvaluateParameters& EvaluationParameters)
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.StatusEffectsToResistances)
	{
		// Check whether the Ability has a matching Status Type Tag (Pair.Key)
		if (UAriaAbilitySystemLibrary::GetStatusEffectType(Spec.GetContext()).MatchesTagExact(Pair.Key))
		{
			// We are working with an Ability that has a Status Effect Ability Modifier

			const FGameplayTag EffectResistanceType = Pair.Value;

			/* == DETERMINE STATUS EFFECT APPLIED == */
#pragma region DetermineStatusEffectApplied
			// Determine whether there was a successful Status Effect Applied
			const float SourceStatusEffectChance = Spec.GetSetByCallerMagnitude(
				GameplayTags.StatusEffect_Chance, false, -1.f);

			// Check for potential Status Effect Resistances
			// Ensure the Resistance Tag exists in TagsToCaptureDefs; if so assign it to 'CaptureDef'
			checkf(InTagsToCaptureDefs.Contains(EffectResistanceType),
			       TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"),
			       *EffectResistanceType.ToString());
			const FGameplayEffectAttributeCaptureDefinition EffectResistCaptureDef = InTagsToCaptureDefs[
				EffectResistanceType];

			float StatusEffectResistance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EffectResistCaptureDef, EvaluationParameters,
			                                                           StatusEffectResistance);
			StatusEffectResistance = FMath::Clamp(StatusEffectResistance, 0.f, 100.f);


			// float TargetStatusEffectResistance = 0.f;
			// const FGameplayTag& ResistanceTag = GameplayTags.StatusEffectsToResistances[StatusEffectType];
			//
			// // Returns the Resistance to the Damage Type for this particular Status Effect
			// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToCaptureDefs[ResistanceTag],
			//                                                            EvaluationParameters,
			//                                                            TargetStatusEffectResistance);
			// TargetStatusEffectResistance = FMath::Max<float>(TargetStatusEffectResistance, 0.f);

			// Calculate the Chance for successfully applying the Status Effect
			//  Note: Formula: Each point of Resistance will take off 1% of the chance that the Status Effect will get applied 
			const float EffectiveStatusEffectChance = SourceStatusEffectChance * (100 - StatusEffectResistance) /
				100;

			// Roll 100-sided die and see whether the Status Effect is applied
			const bool bStatusEffectApplied = FMath::RandRange(1, 100) < EffectiveStatusEffectChance;;
#pragma endregion

			if (bStatusEffectApplied)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Status Effect Applied: %s"), *StatusEffectType.GetTagName().ToString()));

				/* == APPLY STATUS EFFECT == */
#pragma region ApplyStatusEffect
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				// Set the Status Effect-related properties in the Effect Context
				UAriaAbilitySystemLibrary::SetStatusEffectApplied(ContextHandle, true);
				const float StatusEffectDuration = Spec.GetSetByCallerMagnitude(
					GameplayTags.StatusEffect_Duration, false, -1.f);
				const float StatusEffectFrequency = Spec.GetSetByCallerMagnitude(
					GameplayTags.StatusEffect_Frequency, false, -1.f);
				const float DamagedAttributeAmount = Spec.GetSetByCallerMagnitude(
					GameplayTags.StatusEffect_Damage, false, -1.f);
				UAriaAbilitySystemLibrary::SetStatusEffectDuration(ContextHandle, StatusEffectDuration);
				UAriaAbilitySystemLibrary::SetStatusEffectFrequency(ContextHandle, StatusEffectFrequency);
				// If Damaged Attribute is set, Apply the Damaged Attribute Damage amount to the Effect Context
				if (UAriaAbilitySystemLibrary::GetDamagedAttributeType(ContextHandle).IsValid())
				{
					UAriaAbilitySystemLibrary::SetDamagedAttributeAmount(ContextHandle, DamagedAttributeAmount);
				}

#pragma endregion
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	// Used for associating the Gameplay Tags with a specific Capture Definition
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	/* Map the Gameplay Tags to Capture Defs */
#pragma region TagsToCaptureDefsMap
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);

	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_Defense, DamageStatics().DefenseDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_DodgeChance, DamageStatics().DodgeChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance,
	                      DamageStatics().CriticalHitResistanceDef);

	/* Damage Type Resistances */
#pragma region DamageTypeResistances
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Physical, DamageStatics().ResistancePhysicalDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Arcane, DamageStatics().ResistanceArcaneDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Fire, DamageStatics().ResistanceFireDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Lightning, DamageStatics().ResistanceLightningDef);
#pragma endregion

	/* Status Type Resistances */
#pragma region StatusTypeResistances
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Stun, DamageStatics().StatusResistStunDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Burning, DamageStatics().StatusResistBurningDef);
#pragma endregion

#pragma endregion

	/* BoilerPlate_DamageCalcInitializers */
#pragma region BoilerPlate_DamageCalcInitializers
	// Gets some initial data; such as who originally caused the Effect and also who the Effect is targeting at the time of Effect Application
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceAsc ? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatarActor = TargetAsc ? TargetAsc->GetAvatarActor() : nullptr;

	// Get the Character Class Info for the Source Avatar Actor
	const UCharacterClassInfo* CharacterClassInfo = UAriaAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);

	// Get the Character Level for the Source and Target Avatar Actors
	int32 SourceCharacterLevel = 1;
	if (SourceAvatarActor->Implements<UCombatInterface>())
	{
		SourceCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatarActor);
	}
	int32 TargetCharacterLevel = 1;
	if (TargetAvatarActor->Implements<UCombatInterface>())
	{
		TargetCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatarActor);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
#pragma endregion

	/* Apply Status Effects (Debuffs)  */
	ApplyStatusEffects(TagsToCaptureDefs, ExecutionParams, Spec, EvaluationParameters);

	/* Apply Damage */
	float Damage;
	ApplyDamage(TagsToCaptureDefs, ExecutionParams, CharacterClassInfo, SourceCharacterLevel, TargetCharacterLevel,
	            Spec,
	            EffectContextHandle,
	            EvaluationParameters, Damage);

	// Set the Damage Meta-Attribute on the Target
	const FGameplayModifierEvaluatedData EvaluatedData(UAriaAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
