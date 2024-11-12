// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AbilitySystem/Abilities/StatusEffects/StatusEffectNiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AriaCharacterBase.generated.h"

class UPassiveAbilityNiagaraComponent;
// Forward declares the AbilitySystemComponent and Attribute Set
class UAbilitySystemComponent;
class UAttributeSet;

class UWidgetComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAnimMontage;
class UNiagaraSystem;

/**
 * @class AAriaCharacterBase
 * @brief Abstract base class for Aria Characters.
 * It provides functionality for ability systems, combat, health bar, and more.
 *
 * This class serves as a base class for Aria Characters, such as enemies or NPCs, providing common functionality and features.
 * It is an abstract class, meaning that you cannot create instances of this class directly, but rather extend it to create
 * concrete character classes.
 *
 * This class inherits from ACharacter, IAbilitySystemInterface, and ICombatInterface.
 * It also includes several properties and methods for handling ability systems, leveling up, combat, health bars,
 * animations, and more.
 */
UCLASS(Abstract)
class ARIA_API AAriaCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAriaCharacterBase();

	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Delegate for when the Ability System Component has been Registered (see GetOnAscRegistered in Combat Interface section)
	FOnAscRegisteredSignature OnAscRegistered;
	FOnDeathSignature OnDeath;

	// The Override for the IAbilitySystemInterface.GetAbilitySystemComponent() method.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Besides the Getter for the AbilitySystemComponent, it is a good practice
	// to also add a Getter for the Attribute Set.
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/* Health Bar feature */
#pragma region Health Bar feature
	// Delegates used to track when Health and MaxHealth Attributes have been changed
	//  (used for things such as updating a Health Bar, etc.)
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
#pragma endregion

	// Callback function for handling Hit React events
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);

	// Helps track whether or not the Character was hit
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	// Base Walk Speed for the Character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 400.f;

	// Lifespan for the Character after Death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float RemainingTTL = 5.f;

	// This is the Target Actor that an AI Controlled Character (such as Enemies or NPCs) can use to help identify their target
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CombatTarget;

	/* Combat Interface */
#pragma region Combat Interface
	virtual FOnAscRegisteredSignature& GetOnAscRegisteredDelegate() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;

	virtual AActor* GetAvatar_Implementation() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual int32 GetCharacterLevel_Implementation() override;

	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;;
	
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;

	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

	// This stores the list of Attack Montages used by a Character
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedAttackMontage> AttackMontages;

	// Function used for retrieving the list of Attack Montages for the Character
	virtual TArray<FTaggedAttackMontage> GetAttackMontages_Implementation() override;

	// Function used for looking up an Attack Montage by a specific Tag.
	virtual FTaggedAttackMontage
	GetTaggedAttackMontageByTag_Implementation(const FGameplayTag& AttackMontageTag) override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual bool GetIsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bInIsBeingShocked) override;
	
	// Note: The die function specifically handles what happens just on the Server.
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;

	virtual UNiagaraSystem* GetDamagedVFX_Implementation() override;
	virtual USoundBase* GetDamagedSFX_Implementation() override;

	// Gets the Remaining Time to Live for the Character
	virtual float GetRemainingTtl_Implementation() override;

	// ** Summoning **

	// Sets the Character to a Minion Character type
	virtual void SetCharacterAsMinion_Implementation() override;

	// Checks if the Character is a Minion Character type
	virtual bool CheckCharacterIsMinion_Implementation() override;

	// Sets the Minions Owning Character
	virtual void SetMinionOwningCharacter_Implementation(AActor* OwningCharacter) override;

	// Gets/Increments the total number of Minions/Pets belonging to that Character
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override;
#pragma endregion

	// This function is used to handle what happens on all Clients when a Character dies.
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	/* Burn Effect */
#pragma region Burn Effect
	UPROPERTY(ReplicatedUsing=OnRep_Burning, BlueprintReadOnly)
	bool bIsBurning = false;

	UFUNCTION()
	virtual void OnRep_Burning();
	
	void BurningTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);
#pragma endregion
	
	/* Stun Effect */
#pragma region Stun Effect
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UFUNCTION()
	virtual void OnRep_Stunned();
#pragma endregion

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;
	
protected:
	virtual void BeginPlay() override;

	// TObjectPtr is relatively new in Unreal 5.
	//  In a Packaged Build, it behaves the same as a Raw Pointer.
	//  In Unreal Editor, it has some additional features;
	//		Access Tracking (Can track how often a pointer is accessed or dereferenced).
	//		Optional Lazy Load Behavior (An Asset cannot be loaded until it is actually needed or used in the Program).
	// The standard now is moving towards using Object Pointers for Member Variables. 

	// UPROPERTY should wrap all Member Variable Pointers

	// Stores Pointers to the Ability System Component and the Attribute Set.
	//  These pointers will be inherited by all Characters (whether those Characters
	//  are going to construct these in that Character class or not).
	// The Enemy Base class (AriaEnemy.cpp) will be storing and Constructing these directly.
	// The Player class will be storing these on the Player State.
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/* Combat Socket Names */
#pragma region Combat Socket Names
	// Store a pointer to a Skeletal Mesh Component on the Character;
	//	used for the Weapon.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// This will be the name for the tip of a Weapon (where applicable)
	//  This assists in making sure projectiles, spells, etc. spawn at the tip of a Weapon
	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	// This will be the name for the Right Hand of a Character Mesh (where applicable)
	//  This assists in making sure projectiles, spells, etc. spawn from the correct place
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;

	// This will be the name for the Left Hand of a Character Mesh (where applicable)
	//  This assists in making sure projectiles, spells, etc. spawn from the correct place
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;

	// This will be the name for the Tail of a Character Mesh (where applicable)
	//  This assists in making sure projectiles, spells, etc. spawn from the correct place
	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailSocketName;

	// This will be the name for the Head of a Character Mesh (where applicable)
	//  This assists in making sure projectiles, spells, etc. spawn from the correct place
	UPROPERTY(EditAnywhere, Category="Combat")
	FName HeadSocketName;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 CharacterLevel = 1;

	virtual void InitAbilityActorInfo();

	// Defaults the Primary Attributes for the Character
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// Updates (Adjusts accordingly) the Secondary Attributes whenever any Base Attribute changes
	// Important: This class must be initialized only AFTER the DefaultPrimaryAttributes has been initialized.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// Defaults the Vital Attributes for the Character
	// Important: This class must be initialized only AFTER the DefaultSecondaryAttributes has been initialized.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Defaults the Resistances Attributes for the Character
	// Important: This class must be initialized only AFTER the DefaultSecondaryAttributes has been initialized.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultResistancesAttributes;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GamePlayEffectClass, const float EffectLevel) const;
	virtual void InitializeDefaultAttributes() const;
	void InitializeStartupAbilities() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	bool bDead = false;

	/* Dissolve Effects and Functions */
#pragma region Dissolve_Effects_Functions
	// Dissolve material used for the Character Mesh 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	// Dissolve material used for the Character's Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// Handles the actual Dissolve Effect by swapping out the original Material with the dissolve one.
	//   i.e. Creates a Dynamic Material Instance based on the Dissolve Material Instances above
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* WeaponDynamicMaterialInstance);
#pragma endregion

	// The Visual Effect to show whenever a Character takes Damage (such as a Blood Splatter effect)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* DamagedVFX;

	// The Sound Effect to play whenever a Character takes Damage (such as a pained yell)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DamagedSFX;

	// The Visual Effect to show whenever a Character Dies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* DeathVFX;

	// The Sound Effect to play whenever a Character Dies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSFX;

	// The Visual Effect to show whenever a 'Burn' Status Effect has been applied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UStatusEffectNiagaraComponent> StatusEffectBurnNiagaraComponent;
	
	// The Visual Effect to show whenever a 'Stun' Status Effect has been applied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UStatusEffectNiagaraComponent> StatusEffectStunNiagaraComponent;

	/* Summon-related */
#pragma region Summoning
	// Stored whether this Character is a Minion
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summoning")
	bool bMinion = false;

	UPROPERTY(BlueprintReadOnly, Category = "Summoning")
	AActor* MinionOwningCharacter;

	// This keeps track of the total # of Minions/Pets spawned (Summoned) by the Character
	int32 MinionCount = 0;
#pragma endregion
	
	/* Stun Effect */
	//  Applies a Stun Effect; essentially Immobilizing the Character whenever that Character has the 'Stun' Tag
	virtual void StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);

private:
	// These are the Abilities that will be given to the Character from the beginning of the Game.
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// These are the Passive Abilities that will be given to the Character from the beginning of the Game.
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultPassiveAbilities;

	// This stores the Hit React Animation Montage that will be used by the Character
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	/* Passive Ability Effects - Niagara System */
#pragma region PassiveEffectsNiagara
	// Scene Component is used to help control the rotation of the Niagara Component VFX
	//  (i.e. how they should show on the screen)
	//  With the FixedEffectAttachComponent, the Effect will never rotate with
	//   the Character but always stay in the same position
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FixedEffectAttachComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveAbilityNiagaraComponent> BarrierNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveAbilityNiagaraComponent> SiphonHealthNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveAbilityNiagaraComponent> SiphonManaNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveAbilityNiagaraComponent> SiphonStaminaNiagaraComponent;
#pragma endregion
};
