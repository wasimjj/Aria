// Copyright Savoie Interactive Studios


#include "UI/WidgetController/AriaWidgetController.h"

#include "Player/AriaPlayerController.h"
#include "Player/AriaPlayerState.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UAriaWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UAriaWidgetController::BroadcastInitialValues()
{
}

void UAriaWidgetController::BindCallbacksToDependencies()
{
}

void UAriaWidgetController::BroadcastAbilityInfo()
{
	if (!GetAriaAsc()->bStartupAbilitiesGiven) return;

	// Gets information for all the Abilities, looks up their Ability Info, and Broadcasts it to all listening Widgets

	// Create a ForEach Broadcast Delegate to Broadcast the Ability Info for each Ability Spec
	FForEachAbilitySignature BroadcastAbilityInfoDelegate;
	BroadcastAbilityInfoDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		// Gets the AbilityTag from a given Ability Spec
		FAriaAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(
			AriaAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));

		// Gets the Ability Type from a given Ability Spec
		Info.AbilityType = AriaAbilitySystemComponent->GetAbilityTypeTagFromSpec(AbilitySpec);
		
		// Gets the Ability Status from a given Ability Spec
		Info.AbilityStatus = AriaAbilitySystemComponent->GetAbilityStatusTagFromSpec(AbilitySpec);
		
		// Gets the InputActivationTag from a given Ability Spec (where applicable)
		Info.InputActivationTag = AriaAbilitySystemComponent->GetInputActivationTagFromSpec(AbilitySpec);

		// Delegate Broadcasts out to the Widgets
		AbilityInfoDelegate.Broadcast(Info);
	});

	// Calls the ForEach Delegate to Broadcast the Ability Info for each Ability
	AriaAbilitySystemComponent->ForEachAbility(BroadcastAbilityInfoDelegate);
}

AAriaPlayerController* UAriaWidgetController::GetAriaPlayerController()
{
	if (AriaPlayerController == nullptr)
	{
		AriaPlayerController = Cast<AAriaPlayerController>(PlayerController);
	}

	return AriaPlayerController;
}

AAriaPlayerState* UAriaWidgetController::GetAriaPlayerState()
{
	if (AriaPlayerState == nullptr)
	{
		AriaPlayerState = Cast<AAriaPlayerState>(PlayerState);
	}

	return AriaPlayerState;
}

UAriaAbilitySystemComponent* UAriaWidgetController::GetAriaAsc()
{
	if (AriaAbilitySystemComponent == nullptr)
	{
		AriaAbilitySystemComponent = Cast<UAriaAbilitySystemComponent>(AbilitySystemComponent);
	}

	return AriaAbilitySystemComponent;
}

UAriaAttributeSet* UAriaWidgetController::GetAriaAttributeSet()
{
	if (AriaAttributeSet == nullptr)
	{
		AriaAttributeSet = Cast<UAriaAttributeSet>(AttributeSet);
	}

	return AriaAttributeSet;
}
