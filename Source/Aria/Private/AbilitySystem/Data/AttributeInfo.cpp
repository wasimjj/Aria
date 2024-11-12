// Copyright Savoie Interactive Studios


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aria/AriaLogChannels.h"

FAriaAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
                                                           const bool bLogNotFound) const
{
	for (const FAriaAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	// If no AttributeInfo match was found, Log an error (if bLogNotFound = true)
	if (bLogNotFound)
	{
		UE_LOG(LogAria, Error, TEXT("Can't find info for Attribute Tag [%s] on Attribute Info [%s]."),
		       *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAriaAttributeInfo();
}
