// Copyright Savoie Interactive Studios


#include "AbilitySystem/Abilities/AriaShockAbility.h"

#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAriaShockAbility::StoreCursorDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		CursorHitLocation = HitResult.ImpactPoint;
		CursorHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAriaShockAbility::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAriaShockAbility::TraceFirstTarget(const FVector& TargetLocation)
{
	check(OwnerCharacter);

	// Get the Start Location
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));

			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);

			FHitResult HitResult;
			
			// Perform Sphere Trace
			//  Note: Using Sphere Trace for some extra leeway;
			//			More useful because we are trying to capture anything
			//			even remotely in the way of the Shock Beam
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				TargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			// Check if hit anything
			if (HitResult.bBlockingHit)
			{
				CursorHitLocation = HitResult.ImpactPoint;
				CursorHitActor = HitResult.GetActor();
			}
		}
	}

	// Bind OnDeath Delegate to the PrimaryTargetDied event function
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(CursorHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAriaShockAbility::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAriaShockAbility::PrimaryTargetDied);	
		}
	}
}

void UAriaShockAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	// Number of Additional Targets to Attack
	//  (Spawns an additional Shock Beam off of the parent one to each additional Target)
	const int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	
	OutAdditionalTargets = UAriaAbilitySystemLibrary::GetActorsAroundTarget(NumAdditionalTargets,
	                                                                        ShockRadius,
	                                                                        GetAvatarActorFromActorInfo(),
	                                                                        CursorHitActor);

	// Bind OnDeath Delegate to the AdditionalTargetDied event function
	for (AActor* AdditionalTarget : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTarget))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAriaShockAbility::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAriaShockAbility::AdditionalTargetDied);	
			}
		}	
	}
}
