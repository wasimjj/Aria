// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AriaHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilityMenuWidgetController;
class UAriaUserWidget;
struct FWidgetControllerParams;

/**
 * @class AAriaHUD
 *
 * The AAriaHUD class is a subclass of AHUD and represents the HUD (Heads-Up Display) for the game.
 *
 * It provides methods for getting different Widget Controllers; such as the Overlay Widget Controller and
 * the Attribute Menu Widget Controller, as well as initializing the Overlay Widget.
 *
 * Any new Widget Controllers must first be added to this class.
 *
 * @headerfile AriaHUD.h
 */
UCLASS()
class ARIA_API AAriaHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(
		const FWidgetControllerParams& WidgetControllerParams);
	UAbilityMenuWidgetController* GetAbilityMenuWidgetController(
		const FWidgetControllerParams& WidgetControllerParams);

	// Init Overlay will construct the WidgetOverlayController, construct the WidgetOverlay,
	// set the Widget's WidgetController, and add the Widget to the Viewport.
	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
	                 UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

private:
	// This stores the Overlay Widget
	UPROPERTY()
	TObjectPtr<UAriaUserWidget> OverlayWidget;

	// This stores the type of the class for the Widget being created
	// The EditAnywhere allows for setting directly from the HUD Blueprint.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAriaUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	// This needs to be settable from the Blueprint
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	// This gets set from the BP_AriaHUD Blueprint
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAbilityMenuWidgetController> AbilityMenuWidgetController;

	// This gets set from the BP_AriaHUD Blueprint
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbilityMenuWidgetController> AbilityMenuWidgetControllerClass;
};
