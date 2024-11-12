// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AriaUserWidget.generated.h"

/**
 *
 * @brief A class that serves as a base for all (UI) widgets in the system.
 *
 * This class is derived from the UUserWidget class and provides functionality for setting a widget controller
 * and accessing it from blueprints.
 *
 * @see UUserWidget
 *
 */
UCLASS()
class ARIA_API UAriaUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// This function is used to Set the Widget Controller
	// Having this be Blueprint Callable means that we can set the Widget Controller from Blueprints if needed.
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// The Widget Controller is going to need to be accessed from Blueprints.
	//	- As all of our User Widget classes are going to be Widget Blueprints that use this C++ class as their base. 
	UPROPERTY(BlueprintReadOnly)
	// Making this a UObject type makes the WidgetController a lot more versatile.
	TObjectPtr<UObject> WidgetController;

protected:
	// Whenever we set the WidgetController for a given Widget (via SetWidgetController function), we'll also call this Function.
	// Once called, then anything we want to have happen in the Widget Blueprint, in response to the Widget Controller, will happen. 
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
