// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AriaDamageGameplayAbility.h"
#include "AriaShockAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARIA_API UAriaShockAbility : public UAriaDamageGameplayAbility
{
	GENERATED_BODY()

public:

	// Caches the hit result into the CursorHitLocation, CursorHitActor, etc.
	UFUNCTION(BlueprintCallable)
	void StoreCursorDataInfo(const FHitResult& HitResult);

	// Stores the Owner Play Controller and Owner Character data
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable) 
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
	
protected:
	// The location where the Cursor click occured
	UPROPERTY(BlueprintReadWrite)
	FVector CursorHitLocation;

	// The Actor hit during the Cursor click event
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CursorHitActor;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShockTargets = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShockRadius = 150.f;
};
