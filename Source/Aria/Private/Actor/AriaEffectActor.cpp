// Copyright Savoie Interactive Studios


#include "Actor/AriaEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAriaEffectActor::AAriaEffectActor()
{
	// Set this actor to call Tick() every frame
	//   Note: Turned this off to improve performance
	PrimaryActorTick.bCanEverTick = false;

	// Create a scene root component for this actor
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAriaEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

// Function to apply a given Gameplay Effect to the Target Actor
void AAriaEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                           const bool bDestroyImmediately = false)
{
	// If Target is an Enemy and ApplyEffectsToEnemies is false then do nothing
	// Note: Added here because some Actors directly use this function instead of the OnOverlap/EndOverlap below
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	// Get the Ability System Component of the Target Actor
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	// Check if the Target Actor implements the Ability System... if so apply it.
	// If we acquire the Target's ASC (verified they implement the IASC), then we can apply effects to the Target.
	if (TargetAsc == nullptr) return;

	// Check the validity of the provided Gameplay Effect class
	// Forcefully crash if the Gameplay Effect class doesn't exist
	check(GameplayEffectClass);

	// Creates a new Effect Context and returns a Handle ("FGameplayEffectContextHandle") to that new Context
	// Note: The Handle is essentially a Wrapper that holds something (ex: an Effect Context)
	FGameplayEffectContextHandle EffectContext = TargetAsc->MakeEffectContext();

	// This stores the Actor or object that caused the Gameplay Effect
	EffectContext.AddSourceObject(this);

	// Creates a new Spec for a Gameplay Effect
	// Note: Spec refers to a "Gameplay Effect Spec (GES)"
	// Context refers to the Context of the Situation (ex: Who is the Target of the Effect, Who is causing the Effect, Is the Effect a Fire/Lightning Effect, etc.)
	// Note: The Handle is essentially a Wrapper that holds something (ex: an Effect Spec)
	const FGameplayEffectSpecHandle EffectSpec = TargetAsc->MakeOutgoingSpec(
		GameplayEffectClass, ActorLevel, EffectContext);

	// This function does not take a pointer, so it must be de-referenced using the '*' before the pointer name.
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(
		*EffectSpec.Data.Get());

	// Check at Runtime the Duration Policy for a given Effect
	// Map Data Structures are great for linking two values together.
	// "EffectSpec.Data.Get()" returns a raw pointer to GameplayEffectSpec
	// The Gameplay Effect itself is called "Def".
	// Note: Avoid trying to change the Duration policy at Runtime.
	const bool bIsInfinite = EffectSpec.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// When the Gameplay Effect is Infinite, add its Handle and associated Target ASC to the ActiveHandles list.
		// Again; these are only getting stored whenever we plan on Removing an effect.
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAsc);
	}

	// Destroy Actor if Instant or Duration Gameplay Effect and if DestroyOnEffectApplication and Destroy immediately = true
	// Note: bDestroyImmediately was added for those Actors that directly use this function instead of the OnOverlap/EndOverlap below
	if (!bIsInfinite && bDestroyOnEffectApplication && bDestroyImmediately)
	{
		Destroy();
	}
}

// Apply Additional Instant Effects to Target Actor
// Note: The main Instant effect must first be set in Blueprint before any additional Instant Effects will trigger.
void AAriaEffectActor::ApplyAdditionalInsEffectsToTarget(AActor* TargetActor,
                                                         TArray<FInstantGameplayEffect> AdditionalEffects)
{
	for (FInstantGameplayEffect const GameplayEffect : AdditionalEffects)
	{
		if (GameplayEffect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect.InstantGameplayEffectClass);
		}
		if (GameplayEffect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect.InstantGameplayEffectClass);
		}
	}
}

// Apply Additional Duration Effects to Target Actor
// Note: The main Duration effect must first be set in Blueprint before any additional Duration Effects will trigger.
void AAriaEffectActor::ApplyAdditionDurEffectsToTarget(AActor* TargetActor,
                                                       TArray<FDurationGameplayEffect> AdditionalEffects)
{
	for (FDurationGameplayEffect const GameplayEffect : AdditionalEffects)
	{
		if (GameplayEffect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect.DurationGameplayEffectClass);
		}
		if (GameplayEffect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect.DurationGameplayEffectClass);
		}
	}
}

// Apply Additional Infinite Effects to Target Actor
// Note: The main Infinite effect must first be set in Blueprint before any additional Infinite Effects will trigger.
void AAriaEffectActor::ApplyAdditionalInfEffectsToTarget(AActor* TargetActor,
                                                         TArray<FInfiniteGameplayEffect> AdditionalEffects)
{
	for (FInfiniteGameplayEffect const GameplayEffect : AdditionalEffects)
	{
		if (GameplayEffect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect.InfiniteGameplayEffectClass);
		}
		if (GameplayEffect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect.InfiniteGameplayEffectClass);
		}
	}
}

void AAriaEffectActor::OnOverlap(AActor* TargetActor)
{
	// If Target is an Enemy and ApplyEffectsToEnemies is false then do nothing
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	// Apply Instant, Duration, and Infinite Game Effect classes based on their Application Policies
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
		ApplyAdditionalInsEffectsToTarget(TargetActor, InstantGameplayEffects);

		if (bDestroyOnEffectApplication)
		{
			Destroy();
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		ApplyAdditionDurEffectsToTarget(TargetActor, DurationGameplayEffects);

		if (bDestroyOnEffectApplication)
		{
			Destroy();
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		ApplyAdditionalInfEffectsToTarget(TargetActor, InfiniteGameplayEffects);
	}
}

void AAriaEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// If Target is an Enemy and ApplyEffectsToEnemies is false then do nothing
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	// Apply Instant, Duration, and Infinite Game Effect classes based on their Application Policies
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
		ApplyAdditionalInsEffectsToTarget(TargetActor, InstantGameplayEffects);

		if (bDestroyOnEffectApplication)
		{
			Destroy();
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		ApplyAdditionDurEffectsToTarget(TargetActor, DurationGameplayEffects);

		if (bDestroyOnEffectApplication)
		{
			Destroy();
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		ApplyAdditionalInfEffectsToTarget(TargetActor, InfiniteGameplayEffects);
	}

	// Remove Infinite Game Effect handles based on the Removal Policy
	// Playing this outside of the Infinite Policy check above ensures that anything applying Infinite-like Effects gets properly ended
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetAsc)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		// Loops through the Active Effects Map and checks if there is an ASC linking an Active Effect to it
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> const HandlePair : ActiveEffectHandles)
		{
			if (TargetAsc == HandlePair.Value)
			{
				// If found, remove the Active Gameplay Effect
				TargetAsc->RemoveActiveGameplayEffect(HandlePair.Key, 1);

				// Store the HandlePair to be queued for Removal later
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		// Loop through HandlesToRemove and remove them from the ActiveEffectHandles array
		for (auto Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
