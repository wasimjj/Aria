// Copyright Savoie Interactive Studios


#include "AbilitySystem/AsyncTasks/ListenForCooldownChange.h"

#include "AbilitySystemComponent.h"

UListenForCooldownChange* UListenForCooldownChange::WaitForCooldownChange(
	UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UListenForCooldownChange* WaitCooldownChange = NewObject<UListenForCooldownChange>();
	WaitCooldownChange->AbilitySystemComponent = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when a Cooldown effect has ended (i.e. Cooldown Tag has been removed)
	// InCooldownTag is the Tag we would like to listen for
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChange,
		&UListenForCooldownChange::CooldownTagChanged);

	// This lets us know when a Gameplay Effect has been added
	// i.e. To know when a Cooldown Effect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange, &UListenForCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UListenForCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, const int32 NewCount) const
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UListenForCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetAsc,
                                                   const FGameplayEffectSpec& SpecApplied,
                                                   FActiveGameplayEffectHandle ActiveEffectHandle) const
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	// Check if any of the Tags contain our Cooldown Tag
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		// Get Any Gameplay Effects that match this specific Cooldown Tag
		const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			// Get the Greatest Time Remaining
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					// Set the Time Remaining to the Highest Time
					TimeRemaining = TimesRemaining[i];
				}
			}

			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}

void UListenForCooldownChange::EndTask()
{
	if (!IsValid(AbilitySystemComponent)) return;

	// Cleanup
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}
