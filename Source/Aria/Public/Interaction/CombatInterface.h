// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UAnimMontage;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAscRegisteredSignature, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

USTRUCT(BlueprintType)
struct FTaggedAttackMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackMontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackSocketTag;

	// The Visual Effect to show for an Attack Montage (such as Sparks flying from a Spear striking a Metal object)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* AttackMontageImpactVFX = nullptr;

	// The Sound Effect to play for an Attack Montage (such as a Metal grinding sound from a Spear striking a Metal object)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* AttackMontageImpactSFX = nullptr;
};

// This class does not need to be modified.
// Note: Changing this to include Blueprint Type allows us to reference this Interface in a Blueprint
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ICombatInterface
 * @brief Interface for a combat system.
 *
 * This interface provides methods for interacting with a combat system.
 * It includes functions for retrieving information about the character, managing combat targets,
 * performing attacks and abilities, handling death, and managing minions/pets.
 */
class ARIA_API ICombatInterface
{
	GENERATED_BODY()

public:
	// = 0 makes this a Pure Virtual function
	virtual FOnAscRegisteredSignature& GetOnAscRegisteredDelegate() = 0;
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

	// This returns the Owner of the CombatInterface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAvatar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel();

	// Returns the Weapon attached to the Character (when available)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();
	
	// Used for determining the place on the Character to spawn the Ability/Spell or other Particle effect.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	// Gets the Combat Target
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	// Sets the Combat Target
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	// This returns a list of Attack Montages
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedAttackMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedAttackMontage GetTaggedAttackMontageByTag(const FGameplayTag& AttackMontageTag);

	// This stores the Montage animation to use whenever a Character gets hit
	//   BlueprintNativeEvent means that we don't have to mark this as a Virtual Blueprint;
	//    it will automatically generate a virtual native version that exists in C++ and that we can override here in the C++.project.
	//		That is the version that has the '_Implementation' at the end of it.
	// Note: See AriaCharacterBase.h for an example of how this virtual function gets implemented.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	// Getter and Setter for bIsBeingShocked
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsBeingShocked() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInIsBeingShocked);
	
	// This handles Character Death
	virtual void Die(const FVector& DeathImpulse) = 0;

	// Checks whether or not the Character has died
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	// When a Projectile is triggered; this will set the Character or Actor to face the direction the Projectile is travelling.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	// Gets the Niagara VFX to use when the Character is Damaged
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetDamagedVFX();

	// Gets the Sound to use when when the Character is Damaged
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USoundBase* GetDamagedSFX();

	// Gets the Remaining Time to Live for the Character
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetRemainingTtl();

	// ** Summoning **
#pragma region Summoning
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCharacterAsMinion();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckCharacterIsMinion();

	// This sets the Summoner as the Owner of the Minion Character
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetMinionOwningCharacter(AActor* OwningCharacter);

	// Gets the total number of Minions/Pets belonging to that Character
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMinionCount();

	// Increments the total MinionCount by a set amount (default: 1) for that Character after a Summon ability gets used
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncrementMinionCount(int32 Amount);
#pragma endregion

	// Misc
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);
};
