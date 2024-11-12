// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AriaWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

// Delegate used to broadcast the Ability Info out to any Listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAriaAbilityInfo&, Info);

class UAbilitySystemComponent;
class UAttributeSet;
class AAriaPlayerController;
class AAriaPlayerState;
class UAriaAbilitySystemComponent;
class UAriaAttributeSet;

class UAbilityInfo;

// Created a new Struct for use with the four main classes below
// (PlayerController, PlayerState, ASC, AttributeSet)
// BlueprintType added for in case we need to create this Struct in a Blueprint 
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams(APlayerController* PlayerController, APlayerState* PlayerState,
	                        UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
		: PlayerController(PlayerController), PlayerState(PlayerState), AbilitySystemComponent(AbilitySystemComponent),
		  AttributeSet(AttributeSet)
	{
	}

	// Note: For Structs it is a good idea and practice to set their initial values (such as nullptr below).
	// EditAnywhere and BlueprintReadWrite; we are giving full Blueprint access to these Variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * \class UAriaWidgetController
 * \brief The base class for all widget controllers in the game.
 *
 * This class is a base class for widget controllers in the game.
 * It provides functionality for setting widget controller parameters and broadcasting initial values.
 * Widget controllers that inherit from this class can use this base functionality and bind callbacks to
 * dependencies.
 *
 * Widget controllers may need access to certain classes throughout the game,
 * such as APlayerController, APlayerState, UAbilitySystemComponent, and UAttributeSet.
 * These classes can be accessed through the member variables;
 * PlayerController, PlayerState, AbilitySystemComponent, and AttributeSet respectively.
 *
 * \note The class that inherits from this class must provide implementation for SetWidgetControllerParams,
 * BroadcastInitialValues, and BindCallbacksToDependencies.
 *--------------------------------------------------------------
 * AriaWidgetController is going to be responsible for getting any Data (ASC, AttributeSet, Character, etc) from the Model,
 * and Broadcasting it over to any Widgets that have their Controller set to it.
 *
 * Note: The dependencies will be one-way from AriaUserWidget to AriaWidgetController.
 *		The Widget Controller doesn't know what Widgets it is associated with,
 *		but the Widgets themselves will know what Controller they are associated with.
 *
 * Throughout the Game, the Widget Controller will need to control certain types of Widgets.
 *	Examples:
 *		Overlay that shows Health and Mana bars, Gold, Ability icons etc.
 *		Menus: Attribute menus, Spell menus, etc.
 */
UCLASS(BlueprintType, Blueprintable)
// Blueprint Type: Means that we can do things like use it as a Type in the Event Graph, such as if we want to cast to this Class type.
// Blueprintable: Means that that we can make a Blueprint based on this Class
class ARIA_API UAriaWidgetController : public UObject
{
	GENERATED_BODY()

public:
	// Function used for setting the Widget Controller Member Variables below.
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams);

	// Any Widget Controller that inherits this function will be able to broadcast their own values
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	// Any Widget Controller will be able to use this to Bind all of their Callbacks to Dependencies 
	virtual void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category="GAS|Ability Details")
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();

protected:
	/* GAS Core Properties (Player Controller, Player State, ASC, Attribute Set) */
	// Throughout the Game, Our Widget Controllers may need to have access to these classes;
	//   specifically the inherited Aria class representations.
	// This is because any child classes of AriaWidgetController may need to access any of these
	// four main Aria class types; either to get Data from them, or to subscribe to Delegates
	// broadcast by them.
#pragma region GAS Core Properties
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAriaPlayerController> AriaPlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAriaPlayerState> AriaPlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAriaAbilitySystemComponent> AriaAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAriaAttributeSet> AriaAttributeSet;

	AAriaPlayerController* GetAriaPlayerController();
	AAriaPlayerState* GetAriaPlayerState();
	UAriaAbilitySystemComponent* GetAriaAsc();
	UAriaAttributeSet* GetAriaAttributeSet();
#pragma endregion

	// Stores the Ability Info for an Ability (Ability Tag, Ability Description, etc.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
};
