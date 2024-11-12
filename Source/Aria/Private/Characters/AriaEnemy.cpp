// Copyright Savoie Interactive Studios


#include "Characters/AriaEnemy.h"

#include "AriaGameplayTags.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AriaGameplayAbility.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "AI/AriaAIController.h"
#include "Aria/Aria.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"

class UAriaGameplayAbility;

AAriaEnemy::AAriaEnemy()
{
	// Sets up 'Custom' Enemy Mesh collision settings.
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Ensures the Enemies Rotate in a smooth manner instead of being 'snappy'
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	// Adjust the Rotation Rate to give a smoother looking turn (Lower # = Smoother, Higher # = Snappier)
	GetCharacterMovement()->RotationRate.Yaw = 200.f;

	// Constructs the AbilitySystem Component and Attribute Set on the Enemy Base Class
	// Adds a new Component called "AbilitySystemComponent" to Enemy Base. 
	AbilitySystemComponent = CreateDefaultSubobject<UAriaAbilitySystemComponent>("AbilitySystemComponent");
	// Sets AbilitySystemComponent as Replicable.
	AbilitySystemComponent->SetIsReplicated(true);
	// Sets the Replication mode to "Minimal" for the AI-Controlled Enemies
	//  Minimal is used for AI-Controlled Characters
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	// Adds a new Subobject called "AttributeSet" to Enemy Base.
	AttributeSet = CreateDefaultSubobject<UAriaAttributeSet>("AttributeSet");
}

void AAriaEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// AI only matters on the Server (i.e. AI Characters are controlled at the Server) so return out of this function if not the Server
	if (!HasAuthority()) return;
	AriaAIController = Cast<AAriaAIController>(NewController);

	if (IsValid(BehaviorTree))
	{
		// Initialize the Blackboard
		AriaAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		// Run the Behavior Tree (produces the Enemy AI)
		AriaAIController->RunBehaviorTree(BehaviorTree);
		// Default Hit Reacting to false on initial start of the Enemy Behavior Tree
		AriaAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
		// Determines if Possessed Pawn is of type Ranged Attacker

		// Gets the CharacterClassInfo > Class Abilities for this Character; and checks if any are of type Ranged Attack
		//  This is used to determine if the AI should also attempt to use the Ranged Attacker AI branch
		const TArray<TSubclassOf<UGameplayAbility>> ClassAbilities =
			UAriaAbilitySystemLibrary::GetCharacterClassAbilities(this, CharacterClass);

		for (const TSubclassOf<UGameplayAbility>& ClassAbility : ClassAbilities)
		{
			// if (const UAriaGameplayAbility* DefaultAbility = ClassAbility.GetDefaultObject())
			// {
			// 	if (DefaultAbility->bRangedAbility)
			// 	{
			// 		AriaAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), true);
			// 		break;
			// 	}
			// }
		}

		AriaAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),
														   CharacterClass != ECharacterClass::Warrior);
	}
}

void AAriaEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	Super::HitReactTagChanged(CallbackTag, NewCount);

	if (AriaAIController && AriaAIController->GetBlackboardComponent())
	{
		AriaAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAriaEnemy::StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (AriaAIController && AriaAIController->GetBlackboardComponent())
	{
		AriaAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}

void AAriaEnemy::Die(const FVector& DeathImpulse)
{
	// Have them Disappear after a certain amount of time
	SetLifeSpan(RemainingTTL);

	if (AriaAIController)
	{
		// When the Enemy Character dies, the Enemy AI Behavior Tree should stop processing any actions.
		AriaAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}

	Super::Die(DeathImpulse);
}

void AAriaEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAriaEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAriaEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void AAriaEnemy::InitAbilityActorInfo()
{
	// Base functions needed from AriaCharacterBase>InitAbilityActorInfo
	//  (such as setting up the Health Bar)
	Super::InitAbilityActorInfo();

	// Initialize the Ability Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// (Call AbilityActorInfoSet on AriaAbilitySystemComponent)
	Cast<UAriaAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	// Enemy base walk speed
	BaseWalkSpeed = 250.f;

	// If this is from the Server; then call the DefaultAttributes and StartupAbilities functions
	// Fixes an issue when trying to load a Client for Multiplayer
	if (HasAuthority())
	{
		InitializeDefaultAttributes();

		// Give Enemy their Startup Abilities
		UAriaAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	OnAscRegistered.Broadcast(AbilitySystemComponent);

	/* Burning Effect */
	//  Adds a Delegate Callback for any time the Stun Status Tag changes on the Character
	AbilitySystemComponent->RegisterGameplayTagEvent(FAriaGameplayTags::Get().StatusEffect_Negative_Burning, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAriaCharacterBase::BurningTagChanged);

	/* Stun Effect */
	//  Adds a Delegate Callback for any time the Stun Status Tag changes on the Character
	AbilitySystemComponent->RegisterGameplayTagEvent(FAriaGameplayTags::Get().StatusEffect_Negative_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAriaEnemy::StunTagChanged);
}

void AAriaEnemy::InitializeDefaultAttributes() const
{
	UAriaAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, CharacterLevel,
	                                                       AbilitySystemComponent);
}
