// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/AriaProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AriaProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aria/Public/AriaGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UAriaProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAriaProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation,
											 const FVector& ForwardFacingVector,
                                             const FGyroAxisPoint& GyroAxisPoint,
                                             const FGameplayTag SocketTag,
                                             const bool bCalcPitchByDistanceToTarget,
                                             const AActor* HomingTarget)
{
	// Check whether this is the Server
	if (const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); !bIsServer) return;
	
	FVector SourceLocation;
	if (SocketTag.IsValid())
	{
		SourceLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);		
	}
	else
	{
		SourceLocation = GyroAxisPoint.Position;
	}
	
	// if (bCalcPitchByDistanceToTarget)
	// {
	// 	const double DistanceBetweenTargets = FVector::Dist(SocketLocation, ProjectileTargetLocation);
	//
	// 	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("This distance between Targets is: %lf"), DistanceBetweenTargets));
	// }

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SourceLocation);

	// Determines the Rotation of the Projectile
	//  Convert Forward Vector to a Rotation, and then to a Quaternion
	const FQuat ForwardRotationQuat = ForwardFacingVector.ToOrientationQuat();
	FRotator ForwardRotation = ForwardRotationQuat.Rotator();
	ForwardRotation.Pitch = PitchOverride;
	SpawnTransform.SetRotation(ForwardRotation.Quaternion());
	
	// SpawnActorDeferred allows us to set any Properties, etc. before the Projectile has been Spawned
	AAriaProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAriaProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->AbilityModifier = MakeAbilityModifierFromClassDefaults();

	// Adds Homing Capabilities
	if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
	{
		Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
	}
	else
	{
		// Likely targeting a Geometry Actor (such as a Floor or a Wall
		Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
		Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
	}
	Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin, HomingAccelerationMax);
	Projectile->ProjectileMovement->bIsHomingProjectile = bHomingProjectile;
	
	// Finish Spawning the Projectile
	Projectile->FinishSpawning(SpawnTransform);
}
