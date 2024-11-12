// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AriaWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;

struct FAriaAttributeInfo;
struct FGameplayTag;

// Our AttributeMenu Widgets will bind to this Delegate.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAriaAttributeInfo&, Info);

/**
 * @class UAttributeMenuWidgetController
 * @brief Controller class for the attribute menu widget. Inherits from UAriaWidgetController.
 *
 * This class is responsible for managing the attribute menu widget and optionally handling attribute upgrades.
 * It provides functions to bind callbacks to dependencies, broadcast initial attribute values,
 * and upgrade attributes.
 *
 * Attributes can be upgraded using attribute points.
 * The class also defines delegate properties for attribute information and attribute point changes.
 *
 * @see UAriaWidgetController
 */
UCLASS()
class ARIA_API UAttributeMenuWidgetController : public UAriaWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	// Function used for Upgrading Stats from the Attribute Menu using Attribute Points
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAttributeInfo>> AttributeInfoList;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
