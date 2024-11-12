// Copyright Savoie Interactive Studios


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AriaPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	/* AriaPlayerState Callbacks */
#pragma region AriaPlayerState_Callbacks
	GetAriaPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32 NewPointsTotal)
		{
			AttributePointsChangedDelegate.Broadcast(NewPointsTotal);
		}
	);
#pragma endregion

	/* AriaAttributeSet Callbacks */
#pragma region AriaAttributeSet_Callbacks
	// Make sure both the DA_AttributeInfo and DA_ResistanceInfo have both been filled in
	check(AttributeInfoList.Num() == 2);

	for (TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : GetAriaAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}
#pragma endregion
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Make sure both the DA_AttributeInfo and DA_ResistanceInfo have both been filled in
	check(AttributeInfoList.Num() == 2);

	// Loop through the TagsToAttributes Map and Broadcast each Attribute when a Match (FindAttributeInfoForTag) is found
	for (TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : GetAriaAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	/* XP System Feature */
	// Broadcast the initial Attribute Points 
	AttributePointsChangedDelegate.Broadcast(GetAriaPlayerState()->GetAttributePoints());
}

/* XP System Feature */
#pragma region XP System Feature
void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAriaAsc()->UpdateAttribute(AttributeTag);
}
#pragma endregion

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	for (const TObjectPtr<UAttributeInfo> Info : AttributeInfoList)
	{
		FAriaAttributeInfo AttributeDetail = Info->FindAttributeInfoForTag(AttributeTag);
		AttributeDetail.AttributeValue = Attribute.GetNumericValue(AttributeSet);
		AttributeInfoDelegate.Broadcast(AttributeDetail);
	}
}
