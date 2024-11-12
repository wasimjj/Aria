// Copyright Savoie Interactive Studios


#include "Characters/AriaCharacter.h"

#include "AbilitySystemComponent.h"
#include "AriaGameplayTags.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AriaPlayerController.h"
#include "Player/AriaPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AriaHUD.h"

AAriaCharacter::AAriaCharacter()
{
	// Construct the Spring Arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	// Construct the Camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("Top-down Camera");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// This makes the Character Mesh Orient to the same direction as the movement.
	//	Ex: If the Character is moving Right, the Mesh will orient to face right
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Adjust the Rotation Rate (speed of the Mesh rotation)
	//	This will be mostly changed in the Character Blueprint, but this gives a Default value of 400
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// Force the Character Movement to Constrain to Plane
	GetCharacterMovement()->bConstrainToPlane = true;
	// Force the Character to Snap to Plane when the Game is started
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Sets the Default Character Class for the Player (Main) Character
	CharacterClass = ECharacterClass::Elementalist;

	/* XP System Feature (Level Up Effects) */
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Level Up VFX - Niagara Component");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AAriaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info for the Server
	InitAbilityActorInfo();
}

void AAriaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info for the Client
	InitAbilityActorInfo();
}

/* XP Interface */
#pragma region XPInterface
int32 AAriaCharacter::GetXPTotal_Implementation() const
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->GetXP();
}

int32 AAriaCharacter::FindLevelForXPTotal_Implementation(const int32 InXPAmount) const
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->LevelUpInfo->FindLevelBasedOnTotalXP(InXPAmount);
}

void AAriaCharacter::AddXPToXPTotal_Implementation(const int32 InXPAmount)
{
	AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	AriaPlayerState->AddToXP(InXPAmount);
}

void AAriaCharacter::AddToLevel_Implementation(const int32 InLevelAmtToAdd)
{
	AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	AriaPlayerState->AddToLevel(InLevelAmtToAdd);

	if (UAriaAbilitySystemComponent* AriaAsc = Cast<UAriaAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		// TODO: Make this work based on an enum set of flag that sets the Game Type Features to include; Maybe in AriaGameModeBase set those features
		//AriaAsc->UnlockAbilitiesByLevel(AriaPlayerState->GetPlayerLevel());
		AriaAsc->UpdateAbilitiesEligibilityByLevel(AriaPlayerState->GetPlayerLevel());
	}
}


void AAriaCharacter::LevelUp_Implementation()
{
	// Aesthetic/Cosmetic Special Effects and Sounds that occur during Level Up
	MultiCastLevelUpEffects();
}

void AAriaCharacter::MultiCastLevelUpEffects_Implementation() const
{
	// The Visual and Sound Effects to Play whenever the Character Levels Up
	if (LevelUpSFX) UGameplayStatics::PlaySound2D(this, LevelUpSFX);

	if (IsValid(LevelUpNiagaraComponent))
	{
		// Make the Level Up Niagara System orient towards the Top-down Camera
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector LevelUpVFXLocation = LevelUpNiagaraComponent->GetComponentLocation();
		// Returns a Rotation Vector (Used with the Make Rotator)
		const FRotator ToCameraRotation = (CameraLocation - LevelUpVFXLocation).Rotation();
		// Set the World Rotation of the LevelUpNiagaraComponent
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);

		// Passing in true to auto-reset the Niagara System for the next use
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAriaCharacter::GetAttributePointsReward_Implementation(const int32 InPlayerLevel) const
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->LevelUpInfo->LevelUpInformation[InPlayerLevel].AttributePointReward;
}

int32 AAriaCharacter::GetAttributePoints_Implementation()
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->GetAttributePoints();
}

void AAriaCharacter::AddToAttributePoints_Implementation(const int32 InAttributePoints)
{
	AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	AriaPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAriaCharacter::GetAbilityPointsReward_Implementation(const int32 InPlayerLevel) const
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->LevelUpInfo->LevelUpInformation[InPlayerLevel].AbilityPointReward;
}

int32 AAriaCharacter::GetAbilityPoints_Implementation()
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->GetAbilityPoints();
}

void AAriaCharacter::AddToAbilityPoints_Implementation(const int32 InAbilityPoints)
{
	AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	AriaPlayerState->AddToAbilityPoints(InAbilityPoints);
}
#pragma endregion

/* Combat Interface */
#pragma region CombatInterface
int32 AAriaCharacter::GetCharacterLevel_Implementation()
{
	const AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();
	check(AriaPlayerState);

	return AriaPlayerState->GetPlayerLevel();
}
#pragma endregion

/* Targeting Interface */
#pragma region Targeting Interface
void AAriaCharacter::ShowAoETargetingReticle_Implementation(UMaterialInterface* DecalMaterial, const float AreaRadius)
{
	if (AAriaPlayerController* AriaPlayerController = Cast<AAriaPlayerController>(GetController()))
	{
		AriaPlayerController->ShowAoETargetingReticle(DecalMaterial, AreaRadius);
		// Hide the Cursor while the Reticle is active  
		AriaPlayerController->bShowMouseCursor = false;
	}
}

void AAriaCharacter::HideAoETargetingReticle_Implementation()
{
	if (AAriaPlayerController* AriaPlayerController = Cast<AAriaPlayerController>(GetController()))
	{
		AriaPlayerController->HideAoETargetingReticle();
		// Show the Cursor after hiding the Reticle  
		AriaPlayerController->bShowMouseCursor = true;
	}
}
void AAriaCharacter::HighlightActor(){} // Not Used
void AAriaCharacter::UnHighlightActor(){} // Not Used
#pragma endregion

void AAriaCharacter::OnRep_Burning()
{
	if (bIsBurning)
	{
		// Make the Client Activate the Niagara System Component
		StatusEffectBurnNiagaraComponent->Activate();
	}
	else
	{
		StatusEffectBurnNiagaraComponent->Deactivate();
	}
}

void AAriaCharacter::OnRep_Stunned()
{
	if (UAriaAbilitySystemComponent* AriaAsc = Cast<UAriaAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAriaGameplayTags GameplayTags = FAriaGameplayTags::Get();
		
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);

		if (bIsStunned)
		{
			// Sends the Blocked Tags to the Client
			AriaAsc->AddLooseGameplayTags(BlockedTags);
			// Make the Client Activate the Niagara System Component
			StatusEffectStunNiagaraComponent->Activate();
		}
		else
		{
			AriaAsc->RemoveLooseGameplayTags(BlockedTags);
			StatusEffectStunNiagaraComponent->Deactivate();
		}
	}
}

void AAriaCharacter::InitAbilityActorInfo()
{
	AAriaPlayerState* AriaPlayerState = GetPlayerState<AAriaPlayerState>();

	check(AriaPlayerState);
	AriaPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AriaPlayerState, this);

	// (Call AbilityActorInfoSet on AriaAbilitySystemComponent)
	Cast<UAriaAbilitySystemComponent>(AriaPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	// Set the AbilitySystemComponent and AttributeSet pointers
	AbilitySystemComponent = AriaPlayerState->GetAbilitySystemComponent();
	AttributeSet = AriaPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);

	/* Burning Effect */
	AbilitySystemComponent->RegisterGameplayTagEvent(FAriaGameplayTags::Get().StatusEffect_Negative_Burning, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAriaCharacterBase::BurningTagChanged);
	
	/* Stun Effect */
	//  Adds a Delegate Callback for any time the Stun Status Tag changes on the Character
	AbilitySystemComponent->RegisterGameplayTagEvent(FAriaGameplayTags::Get().StatusEffect_Negative_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAriaCharacter::StunTagChanged);
	
	// Initialize the Overlay (Main HUD) if the PlayerController for the Character is valid (i.e. if the Character is a Player)
	if (AAriaPlayerController* AriaPlayerController = Cast<AAriaPlayerController>(GetController()))
	{
		// Check if the current Player Character is the Locally Controlled Player
		if (AAriaHUD* AriaHUD = Cast<AAriaHUD>(AriaPlayerController->GetHUD()))
		{
			AriaHUD->InitOverlay(AriaPlayerController, AriaPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Sets the Initial Attributes for the Player
	InitializeDefaultAttributes();

	// Add the Initial Abilities for the Player
	InitializeStartupAbilities();
}
