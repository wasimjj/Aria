// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AriaDamageGameplayAbility.h"
#include "AriaMeleeAttackGameplayAbility.generated.h"

/**
 * @class UAriaMeleeAttackGameplayAbility
 *
 * This class represents a melee attack gameplay ability that can cause damage to a target actor.
 * It is a subclass of UAriaDamageGameplayAbility.
 *
 * Inherited From:
 *  - UAriaDamageGameplayAbility
 *
 * Inherited By:
 *  None
 *
 * No additional properties or functions are defined in this class.
 * Refer to the base class UAriaDamageGameplayAbility for additional information about the CauseDamage and GetRandomAttackMontage functions.
 */
UCLASS()
class ARIA_API UAriaMeleeAttackGameplayAbility : public UAriaDamageGameplayAbility
{
	GENERATED_BODY()
};
