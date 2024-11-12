// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingTextComponent.generated.h"

/**
 * \class UFloatingTextComponent
 * \brief A component that displays floating text for damage numbers in the game world.
 *
 * This component is derived from UWidgetComponent and provides the functionality to display floating text
 * in the game world, representing damage numbers.
 *
 * It is typically attached to characters or objects that require damage feedback.
 *
 * \note This component needs a blueprint widget, derived from the base BP_FloatingDamageWidget, to be set
 * as the UserWidgetClass in the component.
 */
UCLASS()
class ARIA_API UFloatingTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageFloatingText(float Damage, bool bDodgedHit, bool bBlockedHit, bool bCriticalHit);
};
