// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveAbilityNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARIA_API UPassiveAbilityNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UPassiveAbilityNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveAbilityTag;

protected:
	virtual void BeginPlay() override;

	// Determines when to show or hide the Passive Ability Niagara Effects
	void OnPassiveEffectActivate(const FGameplayTag& InPassiveAbilityTag, bool bActivate);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
