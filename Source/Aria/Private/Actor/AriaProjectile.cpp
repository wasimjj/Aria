// Copyright Savoie Interactive Studios


#include "Actor/AriaProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Aria/Aria.h"

AAriaProjectile::AAriaProjectile()
{
	// Set this actor to call Tick() every frame
	//   Note: Turned this off to improve performance
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	// Add the Sphere Collision Volume to the Components list
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Add the Projectile Movement Component to the Components list
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No Gravity
}

void AAriaProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAriaProjectile::OnSphereOverlap);

	// Set the Projectile Lifetime
	SetLifeSpan(ProjectileLifeSpan);

	// Ensure Projectile Movement gets Replicated
	SetReplicateMovement(true);
	
	// Attach a Sound that will loop and play while the Projectile is moving
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopSound, GetRootComponent());
}

void AAriaProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	// Check if Source Ability System Component is null
	if (AbilityModifier.SourceAsc == nullptr) return;
	
	const AActor* SourceAvatarActor = AbilityModifier.SourceAsc->GetAvatarActor();

	// If the Effect Causer is equal to OtherActor, then return (don't apply the effect)
	if (SourceAvatarActor == OtherActor) return;

	// If the Projectile is hitting a Friendly Actor, then return (don't apply the effect)
	if (!UAriaAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;

	// Trigger the Hit
	if (!bHit) OnHit();

	// Either Sphere overlap will happen first or the Server will replicate down the Destruction of the Actor to the Client.
	//  Note: Only need to apply the Effect at the Server;
	if (HasAuthority())
	{
		// Check if the Other Actor has a Gameplay Ability System Component associated with it
		if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Now that we have the Target; set it on our Damage Effect (via AbilityModifier)
			AbilityModifier.TargetAsc = TargetAsc;
			
			// Apply the Effect to the Target
			//    The Effect will do something (such as affect an Attribute) and then replicate down from the Server to the Client
			UAriaAbilitySystemLibrary::ApplyDamageEffect(AbilityModifier);
		}

		Destroy();
	}
	else bHit = true;
}

void AAriaProjectile::OnHit()
{
	// Play Sound at Impact Location
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	// Play Impact FX at Impact Location
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	// Stop and destroy the Loop Sound on impact
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}

void AAriaProjectile::Destroyed()
{
	// Stop and destroy the Loop Sound on impact
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	
	// If both are false, then the Destruction replication from the Server has happened before the "OnSphereOverlap" event on the Client.
	//  So go ahead and trigger the Impact
	if (!bHit & !HasAuthority()) OnHit();

	Super::Destroyed();
}