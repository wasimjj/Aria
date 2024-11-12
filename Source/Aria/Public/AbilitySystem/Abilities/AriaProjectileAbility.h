// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AriaDamageGameplayAbility.h"
#include "AriaProjectileAbility.generated.h"

class AAriaProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 * @class UAriaProjectileAbility
 * @brief This class represents a specific type of gameplay ability that spawns a projectile when activated.
 *
 * The UAriaProjectileAbility class is a subclass of UAriaDamageGameplayAbility and inherits its properties and methods.
 * It adds the ability to spawn a projectile upon activation.
 *
 * @see UAriaDamageGameplayAbility
 */
UCLASS()
class ARIA_API UAriaProjectileAbility : public UAriaDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAriaProjectile> ProjectileClass;

	// Stores the number of Projectiles to Spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 1;

	// Overrides the pitch for each Projectile
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float PitchOverride = 0.f;
	
	// Homing Projectile Properties
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bHomingProjectile = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMax = 3200.f;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation,
						 const FVector& ForwardFacingVector,
	                     const FGyroAxisPoint& GyroAxisPoint,
	                     const FGameplayTag SocketTag,
	                     const bool bCalcPitchByDistanceToTarget,
	                     const AActor* HomingTarget);
};
