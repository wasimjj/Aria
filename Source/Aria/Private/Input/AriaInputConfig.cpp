// Copyright Savoie Interactive Studios


#include "Input/AriaInputConfig.h"

#include "Aria/AriaLogChannels.h"

const UInputAction* UAriaInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   const bool bLogNotFound) const
{
	for (const FAriaInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAria, Error, TEXT("Can't find Ability Input Action for InputTag [%s], on InputConfig [%s]"),
		       *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
