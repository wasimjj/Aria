// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AriaWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAriaAbilitySystemComponent;
class UAbilityInfo;
struct FOnAttributeChangeData;

// DataTable Row Struct
// Stores information used for displaying the Toaster Notifications to the screen
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	// For any given Gameplay Tag, there will be a Widget that can be created
	//  and added to the Viewport; whenever receiving the Tag in the form of a Gameplay Effect

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAriaUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

// Storing Dynamic Multicast Delegates for Broadcasting values; like Attributes.
// Setting as Dynamic Multicast; means we would like to be able to assign events to them though a Blueprint and
// multiple Blueprints may want to bind to these Delegates.
// This will be used to send a Value out through the Delegate.
//	Ex: Broadcasting the Health value.
// Note: Putting Signature at the end of the Delegate name is just q quick way to identify it as the Delegate type.
// Note: This is the official Delegate but we also bind to these Delegates (seen below on 'FOnAttributeChangedSignature OnHealthChanged')
//  Once the Delegate is Broadcast; then in the Blueprint, we can add the Event listener node and perform other logic from it.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnXPChangedSignature, float, CurrentXPValue, float, TNLXPValue, float,
                                               PercentXP);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * UOverlayWidgetController is a subclass of UAriaWidgetController that is used for managing
 * the overlay widgets in the game.
 * 
 * It provides various functionality for broadcasting initial values and binding callbacks to dependencies.
 *
 * The class contains several member variables that are used with multicast delegates for attributes, messages,
 * ability details, XP, and player level.
 *
 * Protected member variables include UDataTable and UAbilityInfo for managing widget data.
 * The class also includes a generic function GetDataTableRowByTag for finding a row from a DataTable that
 * matches a given tag (currently used with the Attribute Menu widget for displaying Attributes and their values)
 *
 * There are also several protected functions, including OnInitializeStartupAbilities and OnXPChanged,
 * for handling specific events.
 */
UCLASS()
class ARIA_API UOverlayWidgetController : public UAriaWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Member Variables that are used with the Multicast Delegates above
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnXPChangedSignature OnXPChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	void OnXPChanged(int32 NewXPAmount);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag,
	                       const FGameplayTag& AbilityStatus, const FGameplayTag& NewSlot,
	                       const FGameplayTag& OldSlot) const;
};
