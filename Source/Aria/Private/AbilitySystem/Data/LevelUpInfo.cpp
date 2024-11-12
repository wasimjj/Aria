// Copyright Savoie Interactive Studios


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelBasedOnTotalXP(const int32 XPAmount) const
{
	// Starts off at Level 1
	int32 Level = 1;

	bool bSearchingForLvl = true;
	while (bSearchingForLvl)
	{
		// LevelUpInformation(1) = Level 1 Information
		// Note: The first Element in LevelUpInformation corresponds to No Level (Acts as a Placeholder only)
		if (LevelUpInformation.Num() - 1 <= Level) return Level; // Return Level 1

		// Checks to determine how much XP is required to level up at this Level
		if (XPAmount >= LevelUpInformation[Level].LevelUpRequirement)
		{
			// The current XPAmount is greater than or equal to the LevelUpRequirement for the current Level
			//  Increment the Level by 1
			++Level;
		}
		else
		{
			// The current XPAmount is less than the current Level Requirement and
			//  therefore not high enough to advance to another Level
			bSearchingForLvl = false;
		}
	}

	return Level;
}
