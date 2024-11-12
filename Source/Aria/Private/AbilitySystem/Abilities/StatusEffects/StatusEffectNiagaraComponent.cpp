// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/StatusEffects/StatusEffectNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UStatusEffectNiagaraComponent::UStatusEffectNiagaraComponent()
{
	// Prevents the Niagara System from Auto Activating when first created
	bAutoActivate = false;
}

void UStatusEffectNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	// Handle Niagara Activation/Deactivation when Status Effects are Applied/Removed
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		// Bind Delegate to StatusEffectTagChanged
		Asc->RegisterGameplayTagEvent(StatusEffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &UStatusEffectNiagaraComponent::StatusEffectTagChanged);
	}
	// The Asc may not be available yet... Bind a callback for OnAscRegistered and then call the RegisterGameplayTagEvent
	else if (CombatInterface)
	{
		// The Owner implements the CombatInterface
		//   Create a Delegate that we can broadcast whenever the Ability System Component is valid
		CombatInterface->GetOnAscRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InAsc)
		{
			// Bind Delegate to StatusEffectTagChanged
			InAsc->RegisterGameplayTagEvent(StatusEffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &UStatusEffectNiagaraComponent::StatusEffectTagChanged);
		});
	}

	// Handle Deactivation whenever the Owner of this Niagara Component Dies
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UStatusEffectNiagaraComponent::OnOwnerDeath);
	}
}

void UStatusEffectNiagaraComponent::StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive =
		GetOwner()->Implements<UCombatInterface>() &&
		!ICombatInterface::Execute_IsDead(GetOwner());

	if (NewTagCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UStatusEffectNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
