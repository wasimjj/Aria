// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "ExecCalc_Damage.generated.h"

/**
 * UExecCalc_Damage is a gameplay effect execution calculation class that determines how an execution calculation affects other attributes.
 * It is inherited from UGameplayEffectExecutionCalculation.
 *
 * Relevant Attributes:
 * - ArmorPenetrationDef: The attribute representing armor penetration.
 * - CriticalHitChanceDef: The attribute representing critical hit chance.
 * - CriticalHitDamageDef: The attribute representing critical hit damage.
 * - DefenseDef: The attribute representing defense.
 * - DodgeChanceDef: The attribute representing dodge chance.
 * - BlockChanceDef: The attribute representing block chance.
 * - CriticalHitResistanceDef: The attribute representing critical hit resistance.
 * - ResistancePhysicalDef: The attribute representing physical resistance.
 * - ResistanceArcaneDef: The attribute representing arcane resistance.
 * - ResistancePoisonDef: The attribute representing poison resistance.
 * - ResistanceEarthDef: The attribute representing earth resistance.
 * - ResistanceWindDef: The attribute representing wind resistance.
 * - ResistanceWaterDef: The attribute representing water resistance.
 * - ResistanceFireDef: The attribute representing fire resistance.
 * - ResistanceIceDef: The attribute representing ice resistance.
 * - ResistanceLightningDef: The attribute representing lightning resistance.
 * - ResistanceLightDef: The attribute representing light resistance.
 * - ResistanceDarkDef: The attribute representing dark resistance.
 */
UCLASS()
class ARIA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();
	
	static void ApplyDamage(const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs,
							const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                        const UCharacterClassInfo* CharacterClassInfo, int32 SourceCharacterLevel,
	                        int32 TargetCharacterLevel,
	                        const FGameplayEffectSpec& Spec, FGameplayEffectContextHandle EffectContextHandle,
	                        FAggregatorEvaluateParameters EvaluationParameters, float& Damage);

	static void ApplyStatusEffects(const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs,
								   const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                               const FGameplayEffectSpec& Spec,
	                               const FAggregatorEvaluateParameters& EvaluationParameters);

	// Here we decide how this Execution Calculation is going to affect any other Attributes i.e. by setting their values
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
