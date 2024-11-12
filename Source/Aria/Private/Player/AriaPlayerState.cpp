// Copyright Savoie Interactive Studios


#include "Player/AriaPlayerState.h"

#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "AbilitySystem/AriaAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAriaPlayerState::AAriaPlayerState()
{
	// Constructs the AbilitySystem Component and Attribute Set on the Aria Player State Class
	// Adds a new Component called "AbilitySystemComponent" to Aria Player State. 
	AbilitySystemComponent = CreateDefaultSubobject<UAriaAbilitySystemComponent>("AbilitySystemComponent");
	// Sets AbilitySystemComponent as Replicable.
	AbilitySystemComponent->SetIsReplicated(true);
	// Sets the Replication mode to "Mixed" for the Player State
	//  Mixed is used for Player Controllers that are not AI-Controlled
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// Adds a new Subobject called "AttributeSet" to Aria Player State.
	AttributeSet = CreateDefaultSubobject<UAriaAttributeSet>("AttributeSet");

	// This controls how often the server will try to update Clients
	//	 As changes occur on the Server for the Player State, the Server
	//   will be sending updates out to all Clients so they can Sync up
	//   with the Server version.
	//     So any Variables that should be Replicated, they will update and
	//	   the Server will try to meet the Net Update Frequency if it can.
	//		 Normally the Net Update Frequency is low for the Player State
	//		 (like half a second).
	//	     Games like Fortnite which store the GAS on the Player State
	//		 typically use a higher value (recommended around 100.f).
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAriaPlayerState::GetAbilitySystemComponent() const
{
	// Returns the AbilitySystemComponent pointer
	return AbilitySystemComponent;
}

/* XP System Feature */
#pragma region XP System Feature
void AAriaPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAriaPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAriaPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAriaPlayerState::AddToAbilityPoints(int32 InAbilityPoints)
{
	AbilityPoints += InAbilityPoints;
	OnAbilityPointsChangedDelegate.Broadcast(AbilityPoints);
}

void AAriaPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAriaPlayerState::SetLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}
#pragma endregion

void AAriaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* XP System Feature */
#pragma region XP System Feature
	DOREPLIFETIME(AAriaPlayerState, Level);
	DOREPLIFETIME(AAriaPlayerState, XP);
	DOREPLIFETIME(AAriaPlayerState, AttributePoints);
	DOREPLIFETIME(AAriaPlayerState, AbilityPoints);
#pragma endregion
}

/* XP System Feature */
#pragma region XP System Feature
void AAriaPlayerState::OnRep_Level(int32 OldLevel) const
{
	// Added the Broadcast here in order to Broadcast also on the Client side. 
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAriaPlayerState::OnRep_XP(int32 OldXP) const
{
	// Added the Broadcast here in order to Broadcast also on the Client side. 
	OnXPChangedDelegate.Broadcast(XP);
}

void AAriaPlayerState::OnRep_AttributePoints(int32 OldAttributePoints) const
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAriaPlayerState::OnRep_AbilityPoints(int32 OldAbilityPoints) const
{
	OnAbilityPointsChangedDelegate.Broadcast(AbilityPoints);
}
#pragma endregion
