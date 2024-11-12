// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/AriaPassiveGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"

void UAriaPassiveGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Once the Passive Ability becomes Active; binding to the DeactivatePassiveAbilityDelegate
	//  Then the Callback function (ReceivedDeactivateRequest) can be used for ending the Passive Ability
	if (UAriaAbilitySystemComponent* AriaAsc = Cast<UAriaAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AriaAsc->DeactivatePassiveAbilityDelegate.AddUObject(this, &UAriaPassiveGameplayAbility::ReceivedDeactivateRequest);
	}
}

void UAriaPassiveGameplayAbility::ReceivedDeactivateRequest(const FGameplayTag& PassiveAbilityTag)
{
	if (AbilityTags.HasTag(PassiveAbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
