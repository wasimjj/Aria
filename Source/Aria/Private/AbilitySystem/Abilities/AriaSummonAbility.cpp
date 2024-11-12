// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/AriaSummonAbility.h"

TSubclassOf<APawn> UAriaSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
