// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AriaGameplayAbility.h"
#include "AriaPassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARIA_API UAriaPassiveGameplayAbility : public UAriaGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	void ReceivedDeactivateRequest(const FGameplayTag& PassiveAbilityTag);
};
