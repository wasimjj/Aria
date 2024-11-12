// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Characters/AriaCharacterBase.h"
#include "Interaction/TargetingInterface.h"
#include "AriaEnemy.generated.h"

class UBehaviorTree;
class AAriaAIController;

/**
 * The AAriaEnemy class represents an enemy character in the game.
 * It inherits from the AAriaCharacterBase class and implements the ITargetingInterface.
 */
UCLASS()
class ARIA_API AAriaEnemy : public AAriaCharacterBase, public ITargetingInterface
{
	GENERATED_BODY()

public:
	AAriaEnemy();

	virtual void PossessedBy(AController* NewController) override;

	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, const int32 NewCount) override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount) override;

	/* Combat Interface */
#pragma region Combat Interface
	virtual void Die(const FVector& DeathImpulse) override;
#pragma endregion

	/* Targeting Interface */
#pragma region Targeting Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	// AI Logic
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAriaAIController> AriaAIController;
};
