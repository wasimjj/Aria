// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AriaPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

// Handles Broadcasting changes to Player Stats for anything listening for those Broadcasts such as UI Widgets
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);

/**
 * The AAriaPlayerState is a fully replicating class that represents the player state in the game.
 * It inherits from APlayerState and IAbilitySystemInterface.
 * It also manages the XP system for the player.
 *
 * The class provides methods and properties to handle managing the XP system feature;
 * such as adding XP, adding levels, adding attribute points, and adding ability points.
 * It also provides getters for XP, player level, attribute points, and ability points.
 *
 * The class contains a pointer to the AbilitySystemComponent and the AttributeSet,
 * which are needed separately from the AriaCharacterBase class.
 *
 * @see APlayerState
 * @see IAbilitySystemInterface
 * @see UAbilitySystemComponent
 * @see UAttributeSet
 */
UCLASS()
class ARIA_API AAriaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAriaPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// The Override for the IAbilitySystemInterface.GetAbilitySystemComponent() method.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Besides the Getter for the AbilitySystemComponent, it is a good practice
	// to also add a Getter for the Attribute Set.
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/* XP System Feature */
#pragma region XP System Feature
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnAbilityPointsChangedDelegate;

	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetAbilityPoints() const { return AbilityPoints; }

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToAbilityPoints(int32 InAbilityPoints);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
#pragma endregion

protected:
	// Stores Pointers to the Ability System Component and the Attribute Set.
	//   These pointers are needed separately from the AriaCharacterBase class
	//   since the AriaPlayerState does not inherit from AriaCharacterBase.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	/* XP System Feature */
#pragma region XP System Feature
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AbilityPoints)
	int32 AbilityPoints = 0;

	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints) const;

	UFUNCTION()
	void OnRep_AbilityPoints(int32 OldAbilityPoints) const;
#pragma endregion
};
