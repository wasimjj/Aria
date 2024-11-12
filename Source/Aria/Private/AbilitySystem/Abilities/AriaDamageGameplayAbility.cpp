// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/AriaDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAriaDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	// Apply the Primary Attack Modifier
	//  Note: When the Curve Table is set; uses the value from the Curve Table, otherwise just uses the Default Value specified in Damage.Value
	const float ScaledDamage = (Damage.Curve.CurveTable) ? Damage.GetValueAtLevel(GetAbilityLevel()) : Damage.Value;
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	// Check whether the Ability implements Multiple Attack Modifiers
	// if (MultiAbilityModifiers.Num() > 0)
	// {
	// 	// Adds any additional Attack Modifiers (ex: Basic Damage, Elemental Damage, etc.)
	// 	//  Loop over our AriaDamageGameplayAbility > MultiAbilityModifiers TArray and assign
	// 	//  a Set By Caller Magnitude to the above Gameplay Spec for each additional Attack Modifier
	// 	for (FAbilityModifier& Modifier : MultiAbilityModifiers)
	// 	{
	// 		const float MultiDamage = (Modifier.ScaledDamage.GetValue() > 0.0f) ? Modifier.ScaledDamage.GetValueAtLevel(GetAbilityLevel()) : Modifier.Damage;
	// 		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Modifier.DamageType, MultiDamage);
	// 	}
	// }

	// Apply the Gameplay Effect to the Target Actor
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UAriaDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FAbilityModifier UAriaDamageGameplayAbility::MakeAbilityModifierFromClassDefaults(AActor* TargetActor) const
{
	FAbilityModifier AbilityModifier;
	AbilityModifier.WorldContextObject = GetAvatarActorFromActorInfo();
	AbilityModifier.DamageGameplayEffectClass = DamageEffectClass;
	AbilityModifier.SourceAsc = GetAbilitySystemComponentFromActorInfo();
	AbilityModifier.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	AbilityModifier.AbilityLevel = GetAbilityLevel();
	AbilityModifier.DamageType = DamageType;
	AbilityModifier.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	AbilityModifier.KnockbackChance = KnockbackChance;
	AbilityModifier.KnockbackForceMagnitude = KnockbackForceMagnitude;

	// Added Magnitude Buffer to work with lower numbers in the Editor (avoids needing to set super high values like 25000/50000)
	AbilityModifier.DeathImpulseMagnitude = DeathImpulseMagnitude * 50;

	AbilityModifier.StatusEffectType = StatusEffect.StatusEffectType;
	AbilityModifier.StatusEffectChance = StatusEffect.StatusEffectChance;
	AbilityModifier.StatusEffectDurationMin = StatusEffect.StatusEffectDuration.StatusEffectDurationMin;
	AbilityModifier.StatusEffectDurationMax = StatusEffect.StatusEffectDuration.StatusEffectDurationMax;
	AbilityModifier.StatusEffectFrequency = StatusEffect.StatusEffectFrequency;
	AbilityModifier.DamagedAttributeType = StatusEffect.DamageAttribute.AttributeType;
	AbilityModifier.DamagedAttributeAmount = StatusEffect.DamageAttribute.AttributeDamageAmount;
	AbilityModifier.AdditionalDamageTrigger = StatusEffect.AdditionalDamageTriggers.AdditionalDamageTrigger;
	AbilityModifier.AdditionalDamageAmount = StatusEffect.AdditionalDamageTriggers.AdditionalDamageAmount;
	AbilityModifier.RandomlyMissActions = StatusEffect.RandomlyMissActions.RandomlyMiss;
	AbilityModifier.RandomMissPercentage = StatusEffect.RandomlyMissActions.RandomMissPercentage;
	AbilityModifier.RandomlyStopActions = StatusEffect.RandomlyStopActions.RandomlyStopActions;
	AbilityModifier.ReducedActionPercentage = StatusEffect.RandomlyStopActions.ReducedActionPercentage;
	AbilityModifier.StopAllActions = StatusEffect.StopAllActions;
	AbilityModifier.ReducedMovementSpeed = StatusEffect.ReduceMovementSpeed;
	AbilityModifier.bRandomizeMovements = StatusEffect.RandomizeMovements;
	AbilityModifier.bRunFromTarget = StatusEffect.RunFromTarget;
	AbilityModifier.bInstantKillTarget = StatusEffect.InstantKillTarget;
	AbilityModifier.ReducedLevels = StatusEffect.ReduceLevels;
	AbilityModifier.bForceAIControl = StatusEffect.ForceAIControl;

	return AbilityModifier;
}

FAbilityModifier UAriaDamageGameplayAbility::MakeFinalAbilityModifierFromClassDefaults(AActor* TargetActor) const
{
	FAbilityModifier FinalAbilityModifier;
	FinalAbilityModifier.WorldContextObject = GetAvatarActorFromActorInfo();
	FinalAbilityModifier.DamageGameplayEffectClass = DamageEffectClass;
	FinalAbilityModifier.SourceAsc = GetAbilitySystemComponentFromActorInfo();
	FinalAbilityModifier.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	FinalAbilityModifier.AbilityLevel = GetAbilityLevel();
	FinalAbilityModifier.DamageType = FinalDamageType;
	FinalAbilityModifier.BaseDamage = FinalDamage.GetValueAtLevel(GetAbilityLevel());
	FinalAbilityModifier.KnockbackChance = FinalKnockbackChance;
	FinalAbilityModifier.KnockbackForceMagnitude = FinalKnockbackForceMagnitude;

	// Added Magnitude Buffer to work with lower numbers in the Editor (avoids needing to set super high values like 25000/50000)
	FinalAbilityModifier.DeathImpulseMagnitude = FinalDeathImpulseMagnitude * 50;

	FinalAbilityModifier.StatusEffectType = FinalStatusEffect.StatusEffectType;
	FinalAbilityModifier.StatusEffectChance = FinalStatusEffect.StatusEffectChance;
	FinalAbilityModifier.StatusEffectDurationMin = FinalStatusEffect.StatusEffectDuration.StatusEffectDurationMin;
	FinalAbilityModifier.StatusEffectDurationMax = FinalStatusEffect.StatusEffectDuration.StatusEffectDurationMax;
	FinalAbilityModifier.StatusEffectFrequency = FinalStatusEffect.StatusEffectFrequency;
	FinalAbilityModifier.DamagedAttributeType = FinalStatusEffect.DamageAttribute.AttributeType;
	FinalAbilityModifier.DamagedAttributeAmount = FinalStatusEffect.DamageAttribute.AttributeDamageAmount;
	FinalAbilityModifier.AdditionalDamageTrigger = FinalStatusEffect.AdditionalDamageTriggers.AdditionalDamageTrigger;
	FinalAbilityModifier.AdditionalDamageAmount = FinalStatusEffect.AdditionalDamageTriggers.AdditionalDamageAmount;
	FinalAbilityModifier.RandomlyMissActions = FinalStatusEffect.RandomlyMissActions.RandomlyMiss;
	FinalAbilityModifier.RandomMissPercentage = FinalStatusEffect.RandomlyMissActions.RandomMissPercentage;
	FinalAbilityModifier.RandomlyStopActions = FinalStatusEffect.RandomlyStopActions.RandomlyStopActions;
	FinalAbilityModifier.ReducedActionPercentage = FinalStatusEffect.RandomlyStopActions.ReducedActionPercentage;
	FinalAbilityModifier.StopAllActions = FinalStatusEffect.StopAllActions;
	FinalAbilityModifier.ReducedMovementSpeed = FinalStatusEffect.ReduceMovementSpeed;
	FinalAbilityModifier.bRandomizeMovements = FinalStatusEffect.RandomizeMovements;
	FinalAbilityModifier.bRunFromTarget = FinalStatusEffect.RunFromTarget;
	FinalAbilityModifier.bInstantKillTarget = FinalStatusEffect.InstantKillTarget;
	FinalAbilityModifier.ReducedLevels = FinalStatusEffect.ReduceLevels;
	FinalAbilityModifier.bForceAIControl = FinalStatusEffect.ForceAIControl;

	return FinalAbilityModifier;
}

FTaggedAttackMontage UAriaDamageGameplayAbility::GetRandomAttackMontage(
	const TArray<FTaggedAttackMontage>& TaggedAttackMontages)
{
	if (TaggedAttackMontages.Num() > 0)
	{
		const int32 MontageTooUse = FMath::RandRange(0, TaggedAttackMontages.Num() - 1);
		return TaggedAttackMontages[MontageTooUse];
	}

	return FTaggedAttackMontage();
}
