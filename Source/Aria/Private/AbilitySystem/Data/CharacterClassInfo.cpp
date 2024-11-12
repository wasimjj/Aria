// Copyright Savoie Interactive Studios


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassData UCharacterClassInfo::GetCharacterClassInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
