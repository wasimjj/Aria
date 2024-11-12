// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/Abilities/AriaGameplayAbility.h"
#include "AriaSummonAbility.generated.h"

UCLASS()
class ARIA_API UAriaSummonAbility : public UAriaGameplayAbility
{
	GENERATED_BODY()

public:
	// The types of Characters (or Objects) that will be spawned
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	// This is the total number of Actors to Summon whenever the Ability gets used
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	int32 NumMinionsToSummon = 5;
	
	// Applies a small delay interval before actually Spawning the Minions 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	float SpawnDelay = 0.35f;

	// The Visual Effect to show while the Minion is preparing to Spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	UNiagaraSystem* MinionPreSummonVFX;

	// The Visual Effect to show directly after the Minion has Spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	UNiagaraSystem* MinionPostSummonVFX;

	// Function loops through the Minion Classes array and pulls a random one from the list to summon
	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
};
