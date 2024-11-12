// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/AriaGameplayAbility.h"

#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "Algo/RandomShuffle.h"

FString UAriaGameplayAbility::GetAbilityDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.", Level);
}

FString UAriaGameplayAbility::GetAbilityNextLevelDesc(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n"), L"Default Ability Name - Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\nCauses More Damage", Level);
}

FString UAriaGameplayAbility::GetAbilityLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float UAriaGameplayAbility::GetAbilityCost(const float InAbilityLevel) const
{
	float AbilityCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Modifier : CostEffect->Modifiers)
		{
			if(Modifier.Attribute == UAriaAttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, AbilityCost);
				break;
			}
		}
	}
	return AbilityCost;
}

float UAriaGameplayAbility::GetAbilityCooldown(const float InAbilityLevel) const
{
	float AbilityCooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, AbilityCooldown);
	}
	return AbilityCooldown;
}
