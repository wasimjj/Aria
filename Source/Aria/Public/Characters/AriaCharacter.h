// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Characters/AriaCharacterBase.h"
#include "Interaction/XPInterface.h"
#include "Interaction/TargetingInterface.h"
#include "AriaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;

/**
 * AAriaCharacter is a derived class of AAriaCharacterBase and IXPInterface.
 * It represents a player character in the game.
 *
 * @see AAriaCharacterBase
 * @see IXPInterface
 */
UCLASS()
class ARIA_API AAriaCharacter : public AAriaCharacterBase, public IXPInterface, public ITargetingInterface
{
	GENERATED_BODY()

public:
	AAriaCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* XP Interface */
#pragma region XP Interface
	virtual int32 GetXPTotal_Implementation() const override;
	virtual int32 FindLevelForXPTotal_Implementation(const int32 InXPAmount) const override;
	virtual void AddXPToXPTotal_Implementation(const int32 InXPAmount) override;
	virtual void AddToLevel_Implementation(const int32 InLevelAmtToAdd) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 InPlayerLevel) const override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual void AddToAttributePoints_Implementation(const int32 InAttributePoints) override;
	virtual int32 GetAbilityPointsReward_Implementation(const int32 InPlayerLevel) const override;
	virtual int32 GetAbilityPoints_Implementation() override;
	virtual void AddToAbilityPoints_Implementation(const int32 InAbilityPoints) override;
#pragma endregion XP Interface

	/* Combat Interface */
#pragma region Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
#pragma endregion Combat Interface

	/* Targeting Interface */
#pragma region Targeting Interface
	virtual void ShowAoETargetingReticle_Implementation(UMaterialInterface* DecalMaterial, const float AreaRadius) override;
	virtual void HideAoETargetingReticle_Implementation() override;
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
#pragma endregion
	
	// The Visual Effect to show whenever a Character Levels Up
	//  Note: Using TObjectPtr<UNiagaraComponent> as we want this to stay with the Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	// The Sound Effect to play whenever a Character Levels Up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	USoundBase* LevelUpSFX;

	/* Burn Effect */
	virtual void OnRep_Burning() override;
	
	/* Stun Effect */
	virtual void OnRep_Stunned() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	virtual void InitAbilityActorInfo() override;

	/* XP System Feature */
	// Level UP RPC function for the Clients
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastLevelUpEffects() const;
};
