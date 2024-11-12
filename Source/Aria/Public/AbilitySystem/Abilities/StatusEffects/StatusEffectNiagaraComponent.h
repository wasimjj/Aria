// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "StatusEffectNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARIA_API UStatusEffectNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UStatusEffectNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag StatusEffectTag;

protected:
	virtual void BeginPlay() override;

	//  This callback will be bound to a Delegate on the Ability System Component
	//   for whenever a Status Effect Tag changes
	void StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount);

	// Bound Functions (such as through Delegates) must have a UFUNCTION
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
