// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AriaEffectActor.generated.h"

// Forward declarations to reduce compilation dependencies
class UAbilitySystemComponent;
class UGameplayEffect;

// These Policies are used to control when to Apply or Remove Effects
//  A setting will be used to control the Effect Application and Removal
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

// Note: The Removal Policy is really only used for the Infinite Effect type
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	//RemoveOnOverlap (Might have a future use),
	RemoveOnEndOverlap,
	DoNotRemove
};

// Struct for InstantGameplayEffects
USTRUCT(BlueprintType)
struct FInstantGameplayEffect
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
};

// Struct for DurationGameplayEffects
USTRUCT(BlueprintType)
struct FDurationGameplayEffect
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
};

// Struct for InfiniteGameplayEffects
USTRUCT(BlueprintType)
struct FInfiniteGameplayEffect
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};

/**
 * @class AAriaEffectActor
 *
 * @brief The AAriaEffectActor class is used to apply and manage gameplay effects to target actors based on overlap events.
 *
 * The AAriaEffectActor class inherits from AActor and adds functionality for applying and managing gameplay effects.
 * It provides properties for specifying the level of the effect actor, whether to destroy the actor on effect application,
 * whether to apply the effects to enemies, and the classes for instant, duration, and infinite gameplay effects to be applied.
 *
 * The class also provides methods for applying gameplay effects to target actors, as well as additional instant, duration,
 * and infinite gameplay effects. It uses overlap and end overlap events to determine when to apply and remove these effects.
 *
 * @note The methods in this class are Blueprint callable and can be used in Blueprint graphs.
 */
UCLASS()
class ARIA_API AAriaEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAriaEffectActor();

protected:
	virtual void BeginPlay() override;

	// Stores the Level of the Effect Actor
	// This takes advantage of Curve Tables for applying different Scalable Modifiers based on this Level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiple Applied Effects")
	float ActorLevel = 1.f;

	// Determines weather or not to destroy the Actor on Effect application
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | General")
	bool bDestroyOnEffectApplication = false;

	// Determines weather or not Enemies can take advantage of the Effect((s)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | General")
	bool bApplyEffectsToEnemies = false;

	// This stores the Class for an Instant Gameplay Effect that we would like to apply.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Instant")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Instant")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// This stores the Class for a Duration Gameplay Effect that we would like to apply.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Duration")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Duration")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// This stores the Class for an Infinite Gameplay Effect that we would like to apply.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Infinite")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Infinite")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects | Infinite")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	// Property that gives the editor the option to give multiple Instant Gameplay Effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiple Applied Effects | Instant")
	TArray<FInstantGameplayEffect> InstantGameplayEffects;

	// Property that gives the editor the option to give multiple Duration Gameplay Effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiple Applied Effects | Duration")
	TArray<FDurationGameplayEffect> DurationGameplayEffects;

	// Property that gives the editor the option to give multiple Infinite Gameplay Effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiple Applied Effects | Infinite")
	TArray<FInfiniteGameplayEffect> InfiniteGameplayEffects;

	// Stores a Map of the Active Gameplay Effect Handles and their corresponding Ability System Components
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	// This function is used to apply the Gameplay Effect to the Target
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass,
	                         const bool bDestroyImmediately);

	// Additional Applied Gameplay Effects
	// Note: The main Gameplay Effect must first be set in Blueprint for these additional effects to activate.
	void ApplyAdditionalInsEffectsToTarget(AActor* TargetActor, TArray<FInstantGameplayEffect> AdditionalEffects);
	void ApplyAdditionDurEffectsToTarget(AActor* TargetActor, TArray<FDurationGameplayEffect> AdditionalEffects);
	void ApplyAdditionalInfEffectsToTarget(AActor* TargetActor, TArray<FInfiniteGameplayEffect> AdditionalEffects);

	// These functions are used to determine which Effects to apply and when to apply/remove those Effects
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
};
