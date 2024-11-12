// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

// Delegates when added to an Ability Task become Output Execution pins on the Blueprint node
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&,
                                            DataHandle);

/**
 * \brief Class for handling target data under the mouse cursor
 *
 * This class is responsible for handling the target data under the mouse cursor. It provides functionality to
 * create and activate the task, as well as sending and receiving target data.
 *
 * Usage:
 * 1. Create an instance of the UTargetDataUnderMouse class using the static CreateTargetDataUnderMouse() method.
 * 2. Activate the task by calling the Activate() method.
 * 3. If the task is locally controlled, call the SendMouseCursorData() method to send the target data.
 * 4. If the task is not locally controlled, listen for target data by implementing the OnTargetDataReplicatedCallback() method.
 *
 * Example:
 * \code{cpp}
 * // Create the task
 * UTargetDataUnderMouse* Task = UTargetDataUnderMouse::CreateTargetDataUnderMouse(Ability);
 *
 * // Activate the task
 * Task->Activate();
 *
 * // Send target data if locally controlled
 * if (Task->Ability->GetCurrentActorInfo()->IsLocallyControlled())
 * {
 *     Task->SendMouseCursorData();
 * }
 * \endcode
 */
UCLASS()
class ARIA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

private:
	virtual void Activate() override;

	// We will call this function and Broadcast our ValidData Delegate if we are Locally Controlled
	void SendMouseCursorData() const;

	// This function will be called in response to receiving Replicated Target Data
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	                                    FGameplayTag ActivationTag) const;
};
