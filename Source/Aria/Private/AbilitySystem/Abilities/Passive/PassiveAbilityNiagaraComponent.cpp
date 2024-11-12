// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/Passive/PassiveAbilityNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveAbilityNiagaraComponent::UPassiveAbilityNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveAbilityNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	// Handle Niagara Activation/Deactivation when Passive Ability Effects are Applied/Removed
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAriaAbilitySystemComponent* AriaAsc = Cast<UAriaAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		// Bind Delegate to OnPassiveEffectActivate
		AriaAsc->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveAbilityNiagaraComponent::OnPassiveEffectActivate);
	}
	else if (CombatInterface)
	{
		// The Owner implements the CombatInterface
		CombatInterface->GetOnAscRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* Asc)
		{
			if (UAriaAbilitySystemComponent* InnerAriaAsc = Cast<UAriaAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				// Bind Delegate to OnPassiveEffectActivate
				InnerAriaAsc->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveAbilityNiagaraComponent::OnPassiveEffectActivate);
			}
		});
	}

	// Handle Deactivation whenever the Owner of this Niagara Component Dies
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UPassiveAbilityNiagaraComponent::OnOwnerDeath);
	}
}

void UPassiveAbilityNiagaraComponent::OnPassiveEffectActivate(const FGameplayTag& InPassiveAbilityTag, const bool bActivate)
{
	if (InPassiveAbilityTag.MatchesTagExact(PassiveAbilityTag))
	{
		// Don't try to Activate the Passive NiagaraSystem is it is already Active
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveAbilityNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
