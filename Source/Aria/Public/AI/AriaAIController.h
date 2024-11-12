// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AriaAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * @class AAriaAIController
 *
 * @brief The AAriaAIController class is a subclass of AAIController and is responsible for controlling
 * the AI behavior of an enemy character.
 *
 * This class provides functionality to control the behavior tree component of the AI controller, which
 * determines the enemy's actions and decisions in the game.
 *
 * @see AAIController
 */
UCLASS()
class ARIA_API AAriaAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAriaAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
