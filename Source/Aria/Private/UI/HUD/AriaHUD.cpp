// Copyright Savoie Interactive Studios


#include "UI/HUD/AriaHUD.h"

#include "UI/Widget/AriaUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/AbilityMenuWidgetController.h"

/**
 * These Getter function's primary purpose is to check if the Controller has been created;
 * If not then create it and store it in the associated Pointer, else just return it.
 *
 * Note: We should only ever have one Widget Controller (much like a Singleton).
 */
UOverlayWidgetController* AAriaHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAriaHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

UAbilityMenuWidgetController* AAriaHUD::GetAbilityMenuWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (AbilityMenuWidgetController == nullptr)
	{
		AbilityMenuWidgetController = NewObject<UAbilityMenuWidgetController>(
			this, AbilityMenuWidgetControllerClass);
		AbilityMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AbilityMenuWidgetController->BindCallbacksToDependencies();
	}
	return AbilityMenuWidgetController;
}

void AAriaHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
                           UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	// checkf() checks a condition and then prints a formatted string to the Unreal Crash Log if the check fails.
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitialized, please fill out BP_AriaHUD"));
	checkf(OverlayWidgetControllerClass,
	       TEXT("Overlay Widget Controller Class Uninitialized, please fill out BP_AriaHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAriaUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent,
	                                                     AttributeSet);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// Ties the OverlayWidget and OverlayWidget controller together
	// Once this is done, the WBP_Overlay's Blueprint calls the logic in the "Event Widget Controller Set" node.
	OverlayWidget->SetWidgetController(WidgetController);

	// Broadcasts the Initial Values
	OverlayWidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
