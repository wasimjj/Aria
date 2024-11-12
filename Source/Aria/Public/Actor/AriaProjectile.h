// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AriaAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AriaProjectile.generated.h"

class USphereComponent;
class UAudioComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

/**
 * @class AAriaProjectile
 * @brief Represents a projectile in the game world.
 *
 * This class defines a projectile that can be launched by actors in the game.
 * The projectile moves in a straight line and can trigger effects and collisions
 * with other actors.
 *
 * @see AActor
 */
UCLASS()
class ARIA_API AAriaProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAriaProjectile();

	// The Component for handling Projectile Movement
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// The Gameplay Spec; the Projectile will hold a lightweight Gameplay Effect
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FAbilityModifier AbilityModifier;

	// The Sphere used for Collision Detections 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	// Special Component used for getting PAC location clicks
	//  Typically used for the Projectile Homing feature; for whenever a Mouse Click occurs somewhere on the Level Geometry
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

private:
	// The lifetime of the Projectile (i.e. TTL)
	UPROPERTY(EditDefaultsOnly)
	float ProjectileLifeSpan = 10.f;

	// Determines whether or not the Projectile has hit an Actor 
	bool bHit = false;

	// The Niagara System used for the Collision Impact FX
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// The Impact Sound Effect to play on Projectile Impact
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	// The Sound Effect to play while the Projectile is moving
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Triggers the Projectile Hit whenever the Projectile Overlaps with the (Other) Actor
	void OnHit();
};
