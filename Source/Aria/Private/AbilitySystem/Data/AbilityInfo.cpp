// Copyright Savoie Interactive Studios


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aria/AriaLogChannels.h"

FAriaAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, const bool bLogNotFound) const
{
	for (const FAriaAbilityInfo Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAria, Error, TEXT("Can't find info for AbilityTag: [%s] on AbilityInfo: [%s]"),
		       *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAriaAbilityInfo();
}
