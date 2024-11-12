// Copyright Savoie Interactive Studios


#include "Characters/AriaCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "Aria/Aria.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/WIdget/AriaUserWidget.h"
#include "AriaGameplayTags.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/Abilities/Passive/PassiveAbilityNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAriaCharacterBase::AAriaCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Disable to fix an issue where Overlapping Damage Areas would continue to cause damage after leaving them.
	//  Problem was caused by two objects (the mesh and the capsule) both causing Overlap events
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	// Fixes Camera Issue; prevents zooming into another Character or Monster when nearby it
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Needed for the Projectile hits to work
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	/* Weapon Component Setup */
#pragma region Weapon Component Setup
	// Adds the Weapon USkeletalMeshComponent as a Sub Component Object for the Character.
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	// Attach the Weapon to a Socket on the USkeletalMeshComponent
	//	Placing a Socket on one of the Hands, and the Weapon will be attached to that Socket.
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

	// Disable Collision on the Weapon
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#pragma endregion

	/* Health Bar Feature */
#pragma region Weapon Component Setup
	// Add the HealthBar Widget Component
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	// Needed to fix an issue where the Widget was taking over the screen and Player Character couldn't move around.
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
#pragma endregion

	FixedEffectAttachComponent = CreateDefaultSubobject<USceneComponent>("FixedEffectAttachPoint");
	FixedEffectAttachComponent->SetupAttachment(GetRootComponent());
	
	/* Passive Ability Effect Niagara Components */
#pragma region PassiveEfectNiagComp
	BarrierNiagaraComponent = CreateDefaultSubobject<UPassiveAbilityNiagaraComponent>("BarrierNiagaraComponent");
	BarrierNiagaraComponent->SetupAttachment(FixedEffectAttachComponent);

	SiphonHealthNiagaraComponent = CreateDefaultSubobject<UPassiveAbilityNiagaraComponent>("SiphonHealthNiagaraComponent");
	SiphonHealthNiagaraComponent->SetupAttachment(FixedEffectAttachComponent);

	SiphonManaNiagaraComponent = CreateDefaultSubobject<UPassiveAbilityNiagaraComponent>("SiphonManaNiagaraComponent");
	SiphonManaNiagaraComponent->SetupAttachment(FixedEffectAttachComponent);

	SiphonStaminaNiagaraComponent = CreateDefaultSubobject<UPassiveAbilityNiagaraComponent>("SiphonStaminaNiagaraComponent");
	SiphonStaminaNiagaraComponent->SetupAttachment(FixedEffectAttachComponent);
#pragma endregion
	
	/* Status Effect Niagara Components */
#pragma region StatusEfectNiagComp
	// Constructs the 'Burn' Status Effect Niagara Component
	StatusEffectBurnNiagaraComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("BurnNiagaraComponent");
	StatusEffectBurnNiagaraComponent->SetupAttachment(FixedEffectAttachComponent);
	StatusEffectBurnNiagaraComponent->StatusEffectTag = FAriaGameplayTags::Get().StatusEffect_Negative_Burning;

	// Constructs the 'Stun' Status Effect Niagara Component
	StatusEffectStunNiagaraComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("StunNiagaraComponent");
	StatusEffectStunNiagaraComponent->SetupAttachment(FixedEffectAttachComponent);
	StatusEffectStunNiagaraComponent->StatusEffectTag = FAriaGameplayTags::Get().StatusEffect_Negative_Stun;
#pragma endregion
}

void AAriaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This makes it so the Niagara Components attached to the FixedEffectAttachComponent never changes their rotation
	FixedEffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void AAriaCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAriaCharacterBase, bIsBurning);
	DOREPLIFETIME(AAriaCharacterBase, bIsStunned);
	DOREPLIFETIME(AAriaCharacterBase, bIsBeingShocked);
}

void AAriaCharacterBase::OnRep_Burning()
{
}

void AAriaCharacterBase::OnRep_Stunned()
{
}

UAbilitySystemComponent* AAriaCharacterBase::GetAbilitySystemComponent() const
{
	// Returns the AbilitySystemComponent pointer
	return AbilitySystemComponent;
}

/* ICombatInterface */
#pragma region ICombatInterface
FOnAscRegisteredSignature& AAriaCharacterBase::GetOnAscRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeathSignature& AAriaCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

AActor* AAriaCharacterBase::GetAvatar_Implementation()
{
	return this;
}

ECharacterClass AAriaCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

int32 AAriaCharacterBase::GetCharacterLevel_Implementation()
{
	return CharacterLevel;
}

USkeletalMeshComponent* AAriaCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

FVector AAriaCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	// Return correct Socket to use based on the MontageTag
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Head))
	{
		return GetMesh()->GetSocketLocation(HeadSocketName);
	}

	return FVector();
}

AActor* AAriaCharacterBase::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAriaCharacterBase::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

TArray<FTaggedAttackMontage> AAriaCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FTaggedAttackMontage AAriaCharacterBase::GetTaggedAttackMontageByTag_Implementation(
	const FGameplayTag& AttackMontageTag)
{
	for (FTaggedAttackMontage AttackMontage : AttackMontages)
	{
		if (AttackMontage.AttackMontageTag == AttackMontageTag)
		{
			return AttackMontage;
		}
	}
	return FTaggedAttackMontage();
}

UAnimMontage* AAriaCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

bool AAriaCharacterBase::GetIsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

void AAriaCharacterBase::SetIsBeingShocked_Implementation(bool bInIsBeingShocked)
{
	bIsBeingShocked = bInIsBeingShocked;
}

void AAriaCharacterBase::Die(const FVector& DeathImpulse)
{
	// Logic for Detaching the Weapon (Character drops it)
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);

	//TODO: Have the option to either Play a Death Animation or Ragdoll the Character on Death. 
}

void AAriaCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	// Set the Character Death to true
	bDead = true;
	OnDeath.Broadcast(this);

	// The Visual and Sound Effects to Play whenever the Character Dies
	if (DeathSFX) UGameplayStatics::PlaySoundAtLocation(this, DeathSFX, GetActorLocation(), GetActorRotation());
	if (DeathVFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathVFX, GetActorLocation());

	// This makes the Character's Weapon fall to the ground and hit the ground surface
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// Apply Death Impulse to Weapon (TODO: May need to use different calculation (or separate Magnitude property) due to weapons being much lighter)
	//Weapon->AddImpulse(DeathImpulse, NAME_None, true);

	// Make the Character 'Ragdoll'
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// Apply Death Impulse to Character
	GetMesh()->AddImpulse(DeathImpulse);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Make the Character and any Weapons Dissolve
	Dissolve();

	// Subtract from Minion Count (if applicable)
	if (bMinion && MinionOwningCharacter)
	{
		Cast<AAriaCharacterBase>(MinionOwningCharacter)->MinionCount -= 1;

		// Debugging: Shows the new Total after the Minion Count Deduction
		//const FString strVal = FString::SanitizeFloat(Cast<AAriaCharacterBase>(MinionOwningCharacter)->MinionCount);
		//GEngine->AddOnScreenDebugMessage( -1,1.0,FColor::Cyan, strVal );
	}
}

bool AAriaCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

UNiagaraSystem* AAriaCharacterBase::GetDamagedVFX_Implementation()
{
	return DamagedVFX;
}

USoundBase* AAriaCharacterBase::GetDamagedSFX_Implementation()
{
	return DamagedSFX;
}

float AAriaCharacterBase::GetRemainingTtl_Implementation()
{
	return RemainingTTL;
}

void AAriaCharacterBase::SetCharacterAsMinion_Implementation()
{
	bMinion = true;
}

bool AAriaCharacterBase::CheckCharacterIsMinion_Implementation()
{
	return bMinion;
}

void AAriaCharacterBase::SetMinionOwningCharacter_Implementation(AActor* OwningCharacter)
{
	MinionOwningCharacter = OwningCharacter;
}

int32 AAriaCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAriaCharacterBase::IncrementMinionCount_Implementation(const int32 Amount)
{
	MinionCount += Amount;
}
#pragma endregion

void AAriaCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAriaCharacterBase::HitReactTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAriaCharacterBase::BurningTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	bIsBurning = NewCount > 0;
}

void AAriaCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	bIsStunned = NewCount > 0;

	// Immobilize the Character
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;	
}

void AAriaCharacterBase::InitAbilityActorInfo()
{
	/* Health Bar Feature */
#pragma region Health Bar Feature
	// Set the HealthBar's (ProgressBar) Widget Controller
	if (UAriaUserWidget* AriaUserWidget = Cast<UAriaUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		// Now the Widget Controller itself is the AriaCharacterBase class that owns it
		AriaUserWidget->SetWidgetController(this);
	}

	// Bind callbacks to OnHealthChanged and OnMaxHealthChanged
	const UAriaAttributeSet* AriaAttributeSet = Cast<UAriaAttributeSet>(AttributeSet);
	if (AriaAttributeSet)
	{
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AriaAttributeSet->GetHealthAttribute()).
		                             AddLambda(
			                             [this](const FOnAttributeChangeData& Data)
			                             {
				                             OnHealthChanged.Broadcast(Data.NewValue);
			                             });
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AriaAttributeSet->GetMaxHealthAttribute()).
		                             AddLambda(
			                             [this](const FOnAttributeChangeData& Data)
			                             {
				                             OnMaxHealthChanged.Broadcast(Data.NewValue);
			                             });

		// This returns the Delegate that we can bind the callback to HitReactTagChanged
		//  Whenever the Character's AbilitySystemComponent receives the HitReact tag, the HitReactTagChanged event will be called in response
		GetAbilitySystemComponent()->RegisterGameplayTagEvent(FAriaGameplayTags::Get().Effects_HitReact,
		                                                      EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAriaCharacterBase::HitReactTagChanged
		);
	}

	// Broadcast the Initial Health and MaxHealth Values
	OnHealthChanged.Broadcast(AriaAttributeSet->GetHealth());
	OnHealthChanged.Broadcast(AriaAttributeSet->GetMaxHealth());
#pragma endregion
}

void AAriaCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GamePlayEffectClass,
                                           const float EffectLevel) const
{
	// Make sure these are both set!
	check(IsValid(GetAbilitySystemComponent()));
	check(GamePlayEffectClass);

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(
		GamePlayEffectClass, EffectLevel, EffectContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void AAriaCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	ApplyEffectToSelf(DefaultResistancesAttributes, 1.f);
}

void AAriaCharacterBase::InitializeStartupAbilities() const
{
	UAriaAbilitySystemComponent* AriaAbilitySystemComponent = CastChecked<UAriaAbilitySystemComponent>(
		AbilitySystemComponent);

	if (!HasAuthority()) return;

	// If Has Authority (i.e. Is the Server), then Grant these Default Abilities and Default Passive Abilities to the Character
	AriaAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	AriaAbilitySystemComponent->AddCharacterPassiveAbilities(DefaultPassiveAbilities);
}

void AAriaCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		// Create the Dynamic Material Instance
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);

		// Set the Character's Mesh to the Dissolve Material
		//  Note: If the Character's Mesh has multiple Materials, then this will need to be done (i.e. having a Dynamic Material Instance) for each.
		//  UPDATE: Doing this from the EnemyBase Blueprint now... // GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		// Create the Weapon Dynamic Material Instance
		UMaterialInstanceDynamic* WeaponDynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);

		// Set the Weapon's Mesh to the Dissolve Material
		//  Note: If the Character's Mesh has multiple Materials, then this will need to be done (i.e. having a Dynamic Material Instance) for each.
		//  UPDATE: Doing this from the EnemyBase Blueprint now... // Weapon->SetMaterial(0, WeaponDynamicMatInst);
		StartWeaponDissolveTimeline(WeaponDynamicMatInst);
	}
}
