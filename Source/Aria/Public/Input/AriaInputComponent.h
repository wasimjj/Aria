// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AriaInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AriaInputComponent.generated.h"

/**
 * @class UAriaInputComponent
 *
 * The UAriaInputComponent class is a component that is used to bind input actions for abilities.
 * It is a subclass of the UEnhancedInputComponent class.
 *
 * To bind input actions for abilities, you can use the BindAbilityActions method.
 * This method takes in the following template parameters:
 * - UserClass: The class that contains the functions to bind whenever an input is pressed, released, or held.
 * - PressedFuncType: The type of function to bind whenever an input is pressed.
 * - ReleasedFuncType: The type of function to bind whenever an input is released.
 * - HeldFuncType: The type of function to bind whenever an input is held down.
 *
 * The BindAbilityActions method accepts an instance of the UAriaInputConfig class, which is a data asset that
 * stores a list of ability input actions.
 * It also requires an object of the UserClass to bind the functions to, as well as the pressed, released, and
 * held functions themselves.
 *
 * The UAriaInputComponent class also provides additional methods inherited from
 * its superclass, UEnhancedInputComponent, for binding generic input actions.
 *
 * Usage Example:
 *
 * UAriaInputComponent* AriaInputComponent = CastChecked<UAriaInputComponent>(InputComponent);
 * AriaInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAriaPlayerController::OnMoveInputHeld);
 * AriaInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAriaPlayerController::ShiftPressed);
 * AriaInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAriaPlayerController::ShiftReleased);
 * AriaInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased,
 *                                        &ThisClass::AbilityInputTagHeld);
 */
UCLASS()
class ARIA_API UAriaInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Function used to bind Input Actions for Abilities (bind Inputs to Callbacks)
	// PressedFuncType = The type of Function to Bind whenever an Input is Pressed.
	// ReleasedFuncType = The type of Function to Bind whenever an Input is Released.
	// HeldFuncType = The type of Function to Bind whenever an Input is Held down.
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAriaInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAriaInputComponent::BindAbilityActions(const UAriaInputConfig* InputConfig, UserClass* Object,
                                             PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                             HeldFuncType HeldFunc)
{
	check(InputConfig);

	// This will Loop through and bind the Pressed, Released, and Held actions for all of the Input Actions
	for (const FAriaInputAction Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
