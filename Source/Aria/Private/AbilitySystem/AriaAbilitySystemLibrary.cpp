// Copyright Savoie Interactive Studios


#include "AbilitySystem/AriaAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AriaAbilityTypes.h"
#include "AriaGameplayTags.h"
#include "Game/AriaGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AriaPlayerState.h"
#include "UI/WidgetController/AriaWidgetController.h"
#include "UI/HUD/AriaHUD.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/AriaAttributeSet.h"

bool UAriaAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                           FWidgetControllerParams& OutWcParams,
                                                           AAriaHUD*& OutAriaHud)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAriaHud = Cast<AAriaHUD>(PlayerController->GetHUD());
		if (OutAriaHud)
		{
			AAriaPlayerState* AriaPlayerState = PlayerController->GetPlayerState<AAriaPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = AriaPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = AriaPlayerState->GetAttributeSet();
			OutWcParams.PlayerController = PlayerController;
			OutWcParams.PlayerState = AriaPlayerState;
			OutWcParams.AbilitySystemComponent = AbilitySystemComponent;
			OutWcParams.AttributeSet = AttributeSet;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAriaAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WcParams;
	AAriaHUD* AriaHud = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WcParams, AriaHud))
	{
		return AriaHud->GetOverlayWidgetController(WcParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAriaAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WcParams;
	AAriaHUD* AriaHud = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WcParams, AriaHud))
	{
		return AriaHud->GetAttributeMenuWidgetController(WcParams);
	}
	return nullptr;
}

UAbilityMenuWidgetController* UAriaAbilitySystemLibrary::GetAbilityMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WcParams;
	AAriaHUD* AriaHud = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WcParams, AriaHud))
	{
		return AriaHud->GetAbilityMenuWidgetController(WcParams);
	}
	return nullptr;
}

void UAriaAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass,
                                                            float CharacterLevel,
                                                            UAbilitySystemComponent* AbilitySystemComponent)
{
	// Get the Avatar Actor for the Source Object
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	// Retrieve the Character Class Info data asset 
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

	// Retrieve the Default Info for the specified Character Job Class
	const FCharacterClassData ClassDefaultInfo = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);

	// Set the Source Actor for the Primary Attributes Effect Context
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

	// Retrieve the Primary Attributes Gameplay Effect and apply it to the Character (Self)
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes, CharacterLevel, PrimaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	// Set the Source Actor for the Resistances Attributes Effect Context
	FGameplayEffectContextHandle ResistancesAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	ResistancesAttributesContextHandle.AddSourceObject(AvatarActor);

	// Retrieve the Resistances Attributes Gameplay Effect and apply it to the Character (Self)
	const FGameplayEffectSpecHandle ResistancesAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		ClassDefaultInfo.ResistancesAttributes, CharacterLevel, PrimaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*ResistancesAttributesSpecHandle.Data.Get());


	/* Retrieve the Common Secondary and Vital Attributes Gameplay Effects and apply to the Character (Self) */
#pragma region RetrieveCommonSecVitalAttributes
	// Set the Source Actor for the Secondary Attributes Effect Context
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes, CharacterLevel, SecondaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	// Set the Source Actor for the Vital Attributes Effect Context
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, CharacterLevel, VitalAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
#pragma endregion
}

void UAriaAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
                                                     UAbilitySystemComponent* AbilitySystemComponent,
                                                     ECharacterClass CharacterClass)
{
	// Retrieve the Character Class Info data asset 
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	// Loop through the list of Common Abilities stored from the Character Class Info and add them to the ASC
	for (const TSubclassOf<UGameplayAbility> CommonAbility : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	// Loop through the list of Class-specific Abilities stored from the Character Class Info and add them to the ASC
	const FCharacterClassData& ClassInfo = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility> ClassAbility : ClassInfo.ClassAbilities)
	{
		if (AbilitySystemComponent->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ClassAbility,
			                                                        ICombatInterface::Execute_GetCharacterLevel(
				                                                        AbilitySystemComponent->GetAvatarActor()));
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAriaAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAriaGameModeBase* AriaGameMode = Cast<AAriaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AriaGameMode == nullptr) return nullptr;

	return AriaGameMode->CharacterClassInfo;
}

TArray<TSubclassOf<UGameplayAbility>> UAriaAbilitySystemLibrary::GetCharacterClassAbilities(
	const UObject* WorldContextObject,
	const ECharacterClass CharacterClass)
{
	const AAriaGameModeBase* AriaGameMode = Cast<AAriaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AriaGameMode == nullptr) return TArray<TSubclassOf<UGameplayAbility>>();

	UCharacterClassInfo* ClassInfo = AriaGameMode->CharacterClassInfo;

	return TArray<TSubclassOf<UGameplayAbility>>(); //ClassInfo->GetCharacterClassInfo(CharacterClass).ClassAbilities;
}

UAbilityInfo* UAriaAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAriaGameModeBase* AriaGameMode = Cast<AAriaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AriaGameMode == nullptr) return nullptr;

	return AriaGameMode->AbilityInfo;
}

bool UAriaAbilitySystemLibrary::IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsDodgedHit();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetIsDodgedHit(FGameplayEffectContextHandle& EffectContextHandle,
                                               const bool bInIsDodgedHit)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsDodgedHit(bInIsDodgedHit);
	}
}

bool UAriaAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsBlockedHit();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                const bool bInIsBlockedHit)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

bool UAriaAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsCriticalHit();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                 const bool bInIsCriticalHit)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

/* Ability Modifiers */
#pragma region AbilityModifiers

/* Damage */

FGameplayTag UAriaAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		if (AriaEffectContext->GetDamageType().IsValid())
		{
			return *AriaEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

void UAriaAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
                                              const FGameplayTag& InDamageType)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		AriaEffectContext->SetDamageType(DamageType);
	}
}

/* Knockbacks */

FVector UAriaAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

void UAriaAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const FVector& InKnockbackForce)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

FVector UAriaAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

void UAriaAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
                                                const FVector& InDeathImpulse)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

/* Status Effects */

bool UAriaAbilitySystemLibrary::IsStatusEffectApplied(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsStatusEffectApplied();
	}
	return false;
}

inline void UAriaAbilitySystemLibrary::SetStatusEffectApplied(FGameplayEffectContextHandle& EffectContextHandle,
                                                              const bool bInStatusEffectApplied)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsStatusEffectApplied(bInStatusEffectApplied);
	}
}

FGameplayTag UAriaAbilitySystemLibrary::GetStatusEffectType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		if (AriaEffectContext->GetStatusEffectType().IsValid())
		{
			return *AriaEffectContext->GetStatusEffectType();
		}
	}
	return FGameplayTag();
}

void UAriaAbilitySystemLibrary::SetStatusEffectType(FGameplayEffectContextHandle& EffectContextHandle,
                                                    const FGameplayTag& InStatusEffectType)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		const TSharedPtr<FGameplayTag> StatusEffectType = MakeShared<FGameplayTag>(InStatusEffectType);
		AriaEffectContext->SetStatusEffectType(StatusEffectType);
	}
}

float UAriaAbilitySystemLibrary::GetStatusEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetStatusEffectDuration();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetStatusEffectDuration(FGameplayEffectContextHandle& EffectContextHandle,
                                                        const float InStatusEffectDuration)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetStatusEffectDuration(InStatusEffectDuration);
	}
}

float UAriaAbilitySystemLibrary::GetStatusEffectFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetStatusEffectFrequency();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetStatusEffectFrequency(FGameplayEffectContextHandle& EffectContextHandle,
                                                         const float InStatusEffectFrequency)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetStatusEffectFrequency(InStatusEffectFrequency);
	}
}

FGameplayTag UAriaAbilitySystemLibrary::GetDamagedAttributeType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		if (AriaEffectContext->GetDamagedAttributeType().IsValid())
		{
			return *AriaEffectContext->GetDamagedAttributeType();
		}
	}
	return FGameplayTag();
}

void UAriaAbilitySystemLibrary::SetDamagedAttributeType(FGameplayEffectContextHandle& EffectContextHandle,
                                                        const FGameplayTag& InDamagedAttributeType)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		const TSharedPtr<FGameplayTag> DamagedAttributeType = MakeShared<FGameplayTag>(InDamagedAttributeType);
		AriaEffectContext->SetDamagedAttributeType(DamagedAttributeType);
	}
}

float UAriaAbilitySystemLibrary::GetDamagedAttributeAmount(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetDamagedAttributeAmount();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetDamagedAttributeAmount(FGameplayEffectContextHandle& EffectContextHandle,
                                                          const float InDamagedAttributeAmount)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetDamagedAttributeAmount(InDamagedAttributeAmount);
	}
}

EAdditionalDamageTriggers UAriaAbilitySystemLibrary::GetAdditionalDamageTrigger(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetAdditionalDamageTrigger();
	}
	return EAdditionalDamageTriggers::Disabled;
}

void UAriaAbilitySystemLibrary::SetAdditionalDamageTrigger(FGameplayEffectContextHandle& EffectContextHandle,
                                                           const EAdditionalDamageTriggers InAdditionalDamageTrigger)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetAdditionalDamageTrigger(InAdditionalDamageTrigger);
	}
}

float UAriaAbilitySystemLibrary::GetAdditionalDamageAmount(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetAdditionalDamageAmount();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetAdditionalDamageAmount(FGameplayEffectContextHandle& EffectContextHandle,
                                                          const float InAdditionalDamageAmount)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetAdditionalDamageAmount(InAdditionalDamageAmount);
	}
}

ERandomlyMissAnyActions UAriaAbilitySystemLibrary::GetRandomlyMissActions(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetRandomlyMissActions();
	}
	return ERandomlyMissAnyActions::Disabled;
}

void UAriaAbilitySystemLibrary::SetRandomlyMissActions(FGameplayEffectContextHandle& EffectContextHandle,
                                                       const ERandomlyMissAnyActions InRandomlyMissActions)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetRandomlyMissActions(InRandomlyMissActions);
	}
}

float UAriaAbilitySystemLibrary::GetRandomMissPercentage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetRandomMissPercentage();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetRandomMissPercentage(FGameplayEffectContextHandle& EffectContextHandle,
                                                        const float InRandomMissPercentage)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetRandomMissPercentage(InRandomMissPercentage);
	}
}

ERandomlyStopAnyActions UAriaAbilitySystemLibrary::GetRandomlyStopActions(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetRandomlyStopActions();
	}
	return ERandomlyStopAnyActions::Disabled;
}

void UAriaAbilitySystemLibrary::SetRandomlyStopActions(FGameplayEffectContextHandle& EffectContextHandle,
                                                       const ERandomlyStopAnyActions InRandomlyStopActions)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetRandomlyStopActions(InRandomlyStopActions);
	}
}

float UAriaAbilitySystemLibrary::GetReducedActionPercentage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetReducedActionPercentage();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetReducedActionPercentage(FGameplayEffectContextHandle& EffectContextHandle,
                                                           const float InReducedActionPercentage)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetReducedActionPercentage(InReducedActionPercentage);
	}
}

EStopAllActions UAriaAbilitySystemLibrary::GetStopAllActions(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetStopAllActions();
	}
	return EStopAllActions::Disabled;
}

void UAriaAbilitySystemLibrary::SetStopAllActions(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const EStopAllActions InStopAllActions)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetStopAllActions(InStopAllActions);
	}
}

float UAriaAbilitySystemLibrary::GetReducedMovementSpeed(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetRReducedMovementSpeed();
	}
	return 0.f;
}

void UAriaAbilitySystemLibrary::SetReducedMovementSpeed(FGameplayEffectContextHandle& EffectContextHandle,
                                                        const float InReducedMovementSpeed)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetReducedMovementSpeed(InReducedMovementSpeed);
	}
}

bool UAriaAbilitySystemLibrary::IsRandomizeMovements(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsRandomizeMovements();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetRandomizeMovements(FGameplayEffectContextHandle& EffectContextHandle,
                                                      const bool bInRandomizeMovements)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsRandomizeMovements(bInRandomizeMovements);
	}
}

bool UAriaAbilitySystemLibrary::IsRunFromTarget(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsRunFromTarget();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetRunFromTarget(FGameplayEffectContextHandle& EffectContextHandle,
                                                 const bool bInRunFromTarget)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsRunFromTarget(bInRunFromTarget);
	}
}

bool UAriaAbilitySystemLibrary::IsInstantKillTarget(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsInstantKillTarget();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetInstantKillTarget(FGameplayEffectContextHandle& EffectContextHandle,
                                                     const bool bInInstantKillTarget)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsInstantKillTarget(bInInstantKillTarget);
	}
}

int32 UAriaAbilitySystemLibrary::GetReducedLevels(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->GetReducedLevels();
	}
	return 0;
}

void UAriaAbilitySystemLibrary::SetReducedLevels(FGameplayEffectContextHandle& EffectContextHandle,
                                                 const int32 InReducedLevels)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetReducedLevels(InReducedLevels);
	}
}

bool UAriaAbilitySystemLibrary::IsForceAIControl(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAriaGameplayEffectContext* AriaEffectContext = static_cast<const FAriaGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AriaEffectContext->IsForceAIControl();
	}
	return false;
}

void UAriaAbilitySystemLibrary::SetForceAIControl(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const bool bInForceAIControl)
{
	if (FAriaGameplayEffectContext* AriaEffectContext = static_cast<FAriaGameplayEffectContext*>(EffectContextHandle.
		Get()))
	{
		AriaEffectContext->SetIsIForceAIControl(bInForceAIControl);
	}
}
#pragma endregion

void UAriaAbilitySystemLibrary::GetLiveActorsWithinRadius(const UObject* WorldContextObject,
                                                          TArray<AActor*>& OutOverlappingActors,
                                                          const TArray<AActor*>& ActorsToIgnore,
                                                          const float Radius,
                                                          const FVector& SphereOriginLoc,
                                                          const bool bDrawDebuggingSphere,
                                                          const float DebugSphereLifetime)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	// query scene to see what we hit
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World)
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOriginLoc, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(
				OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()));
			}
		}
	}

	if (bDrawDebuggingSphere)
	{
		DrawDebugSphere(World, SphereOriginLoc, Radius, 12, FColor::Red, false, DebugSphereLifetime, -1, .5f);
	}
}

void UAriaAbilitySystemLibrary::GetClosestActors(int32 MaxActors,
												 const TArray<AActor*>& ActorsWithinRadius,
												 TArray<AActor*>& OutClosestActors,
												 const FVector& Origin)
{
	// If the (Live) Actors within the Radius are <= the MaxActors then they are all
	//  the closest actors and there's no need to do math to look this up
	if (ActorsWithinRadius.Num() <= MaxActors)
	{
		OutClosestActors = ActorsWithinRadius;
		return;
	}

	// Check for the Closest Actors
	TArray<AActor*> ActorsToCheck = ActorsWithinRadius;
	int32 NumActorsFound = 0;

	while (NumActorsFound < MaxActors)
	{
		if (ActorsToCheck.Num() == 0) break;
		
		double ClosestDistance = TNumericLimits<double>::Max(); // Sets to the highest value possible
		AActor* ClosestActor;
		for (AActor* PotentialActor : ActorsToCheck)
		{
			// This is the Distance between the Potential Actor and the Origin
			const double DistanceToOrigin = (PotentialActor->GetActorLocation() - Origin).Length();
			if (DistanceToOrigin < ClosestDistance)
			{
				ClosestDistance = DistanceToOrigin;
				ClosestActor = PotentialActor;
			}
		}

		// Remove the ClosestActor from ActorsToCheck and add it to the OutClosestActors
		ActorsToCheck.Remove(ClosestActor);
		OutClosestActors.AddUnique(ClosestActor);
		
		++NumActorsFound;
	}
}

TArray<AActor*> UAriaAbilitySystemLibrary::GetActorsAroundTarget(const int32 MaxSurroundingActors,
                                                                 const float Radius,
                                                                 const AActor* SourceActor,
                                                                 const AActor* TargetActor,
                                                                 const FVector TargetLocation,
                                                                 const bool bIncludeSourceActor,
                                                                 const bool bIncludeTargetActor,
                                                                 const bool bDrawDebugSpheres)
{
	TArray<AActor*> SurroundingActors;

	// Determine the source of the Target Location
	const FVector TargetOriginLocation = TargetActor ? TargetActor->GetActorLocation() : TargetLocation;

	// Determine whether to include the Source and/or Target Actors in the result
	TArray<AActor*> ActorsToIgnore;
	if (!bIncludeSourceActor) ActorsToIgnore.Add(const_cast<AActor*>(SourceActor));
	if (!bIncludeTargetActor && TargetActor) ActorsToIgnore.Add(const_cast<AActor*>(TargetActor));

	TArray<AActor*> OverlappingActors;

	GetLiveActorsWithinRadius(
		SourceActor,
		OverlappingActors,
		ActorsToIgnore,
		Radius,
		TargetOriginLocation);

	GetClosestActors(
		MaxSurroundingActors,
		OverlappingActors,
		SurroundingActors,
		TargetOriginLocation);

	// Debugging
#pragma region SurroundingActorDebugSpheres
	if (bDrawDebugSpheres && SourceActor)
	{
		if (const UWorld* World = SourceActor->GetWorld())
		{
			// Sphere from the Original Point (Target Location) to the total Search Radius
			DrawDebugSphere(World, TargetOriginLocation, Radius, 12, FColor::White, false, 1.5f);
			
			for (const AActor* AdditionalTarget : SurroundingActors)
			{
				DrawDebugSphere(World, AdditionalTarget->GetActorLocation(), 50.f, 12, FColor::Red, false, 1.5f);
			}
		}
	}
#pragma endregion
	
	return SurroundingActors;
}

void UAriaAbilitySystemLibrary::GetGroundSurfacePoint(const UObject* WorldContextObject,
                                                      const FGyroAxisPoint& GyroPoint,
                                                      const float MinDistAboveSurface,
                                                      const float MaxDistAboveSurface,
                                                      const float MinDistBelowSurface,
                                                      const float MaxDistBelowSurface,
                                                      const TArray<AActor*>& ActorsToIgnore,
                                                      const bool bApplyPointNormalRotations,
                                                      const float NormalRangeTolerance,
                                                      const bool bDrawDebugLineTrace,
                                                      FGyroAxisPoint& OutGyroSurfacePt)
{
	if (!WorldContextObject) return;
	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return;

	constexpr float TraceDistanceAbove = 1000.f;
	constexpr float TraceDistanceBelow = 1000.f;

	// Start at a location far above the cursor impact location
	const FVector Start = FVector(GyroPoint.Position.X, GyroPoint.Position.Y, GyroPoint.Position.Z + TraceDistanceAbove);
	// Trace directly downward through the cursor impact location
	const FVector End = FVector(GyroPoint.Position.X, GyroPoint.Position.Y, GyroPoint.Position.Z - TraceDistanceBelow);

	if (bDrawDebugLineTrace) DrawDebugLine(World, Start, End, FColor::Yellow, false, 2.f, 0, 8.f);
	
	FHitResult HitResult;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);
	
	constexpr float TraceSphereRadius = 5.0f;
	
	if (World->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(TraceSphereRadius), CollisionParams))
	{
		if (HitResult.bBlockingHit)
		{
			const FVector HitLocation = HitResult.Location;

			// Generate random Z value above surface within specified range
			const float SurfaceRandomZAbove = FMath::FRandRange(MinDistAboveSurface, MaxDistAboveSurface); 

			// Generate random Z value below surface within specified range
			const float SurfaceRandomZBelow = FMath::FRandRange(MinDistBelowSurface, MaxDistBelowSurface);

			// Create a randomized Z-location that could be above or below the surface
			FVector RandomSurfacePtPos = HitLocation;

			// Check provided ranges and set Z accordingly
			if (MinDistAboveSurface == 0.0f && MaxDistAboveSurface == 0.0f
				&& MinDistBelowSurface == 0.0f && MaxDistBelowSurface == 0.0f) {
				// Both ranges are zero, use original hit location Z
				RandomSurfacePtPos.Z = HitLocation.Z;
			} 
			else if (MinDistAboveSurface == 0.0f && MaxDistAboveSurface == 0.0f) {
				// Only below range is set
				RandomSurfacePtPos.Z = HitLocation.Z - SurfaceRandomZBelow;
			} 
			else if (MinDistBelowSurface == 0.0f && MaxDistBelowSurface == 0.0f) {
				// Only above range is set
				RandomSurfacePtPos.Z = HitLocation.Z + SurfaceRandomZAbove;
			} 
			else {
				// Both above and below range are set
				if (FMath::FRandRange(0.0f, 1.0f) > 0.5f) {
					// Add the above random value
					RandomSurfacePtPos.Z = HitLocation.Z + SurfaceRandomZAbove;
				} else {
					// Subtract the below random value
					RandomSurfacePtPos.Z = HitLocation.Z - SurfaceRandomZBelow;
				}
			}

			// Check if RandomSurfacePtPos.Z is within 25.f of HitLocation.Z
			const bool bNormalWithinRange = FMath::Abs(RandomSurfacePtPos.Z - HitLocation.Z) <= NormalRangeTolerance;

			const FRotator HitNormalRotation = (bApplyPointNormalRotations && bNormalWithinRange)
				? FGyroAxisPoint::CalculateRotationFromNormal(HitResult.Normal)
				: GyroPoint.Rotation;
			
			// Set the Ground Position and Rotation for the GyroSurfacePoint
			OutGyroSurfacePt.Position = RandomSurfacePtPos;
			OutGyroSurfacePt.Rotation = HitNormalRotation;

			// Optionally draw a debug hit location and rotation line at the Point
			if (bDrawDebugLineTrace)
			{
				DrawDebugSphere(World, RandomSurfacePtPos, 15.f, 12, FColor::Orange, false, 8.5f);
				
				if (bApplyPointNormalRotations)
				{
					DrawDebugLine(World, HitLocation, HitLocation + HitResult.Normal * TraceDistanceAbove, FColor::Green, false, 8.f, 0, TraceSphereRadius);					DrawDebugSphere(World, HitLocation, 10.f, 12, FColor::Green, false, 8.f);
				}
			}
		}
	}
}

	/* GyroAxisPoint Generate Patterns */
#pragma region GyroAxisPt_GenPatterns
void UAriaAbilitySystemLibrary::CreateGyroAxisPtSingle(const UObject* WorldContextObject,
                                                       const FVector& PointOriginLoc,
                                                       const bool bDebugShowPointLocations,
                                                       const bool bDebugShowPointRotations,
                                                       FGyroAxisPoint& OutGyroSinglePt)
{
	const FVector NewPointPosition = PointOriginLoc;
	const FRotator NewPointRotation = FRotator::ZeroRotator; // Aligns forward vector with Z-upwards direction
	OutGyroSinglePt.Position = NewPointPosition;
	OutGyroSinglePt.Rotation = NewPointRotation;

	GyroAxisPointDebugHelpers(WorldContextObject,
	                          OutGyroSinglePt,
	                          false,
	                          bDebugShowPointLocations,
	                          bDebugShowPointRotations);
}

void UAriaAbilitySystemLibrary::CreateGyroAxisPtDirectioned(const UObject* WorldContextObject,
                                                            const FVector& PointOriginLoc,
                                                            const FVector& ForwardDirection,
                                                            const int32 NumPoints,
                                                            const float DistanceBetweenPoints,
                                                            const bool bDebugShowSourceToTargetDir,
                                                            const bool bDebugShowPointLocations,
                                                            const bool bDebugShowPointRotations,
                                                            TArray<FGyroAxisPoint>& OutGyroDirectionedPts)
{
	// Ensure there is at least one point
	if (NumPoints <= 0)
	{
		return;
	}

	for (int32 i = 0; i < NumPoints; i++)
	{
		FVector Offset = ForwardDirection * DistanceBetweenPoints * i; // Scale by index
		const FVector NewPointPosition = PointOriginLoc + Offset;
		const FRotator NewPointRotation = FRotator::ZeroRotator;
		FGyroAxisPoint GyroLinePt(NewPointPosition, NewPointRotation);
		OutGyroDirectionedPts.Add(GyroLinePt);

		GyroAxisPointDebugHelpers(WorldContextObject,
		                          GyroLinePt,
		                          false,
		                          bDebugShowPointLocations,
		                          bDebugShowPointRotations);
	}

	// Shows Source Forward Direction
	GyroAxisPointDebugHelpers(WorldContextObject,
	                          FGyroAxisPoint(),
	                          false,
	                          false,
	                          false,
	                          false,
	                          bDebugShowSourceToTargetDir,
	                          PointOriginLoc,
	                          ForwardDirection);
}

void UAriaAbilitySystemLibrary::CreateGyroAxisPtSpread(const UObject* WorldContextObject,
                                                       const FVector& PointOriginLoc,
                                                       const FVector& ForwardDirection,
                                                       const int32 NumPoints,
                                                       const float SpreadRadius,
                                                       const FVector& SpreadAxisRotation,
                                                       const float FixedDistFromSource,
                                                       const float MinDistFromSource,
                                                       const float MaxDistFromSource,
                                                       const float SoftenFixedConeRadius,
                                                       const bool bUseDeltaAngleAsPtForward,
                                                       const bool bDebugShowSourceToTargetDir,
                                                       const bool bDebugShowPointGenRadius,
                                                       const bool bDebugShowPointLocations,
                                                       const bool bDebugShowPointRotations,
                                                       TArray<FGyroAxisPoint>& OutGyroRadialPts)
{
	// Ensure there is at least one point
	if (NumPoints <= 0)
	{
		return;
	}

	if (!WorldContextObject) return;
	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return;

	if (NumPoints > 1)
	{
#pragma region GetSpreadRadius_DebugHelper
		GyroAxisPointDebugHelpers(WorldContextObject,
		                          FGyroAxisPoint(),
		                          false,
		                          false,
		                          false,
		                          bDebugShowPointGenRadius,
		                          bDebugShowSourceToTargetDir,
		                          PointOriginLoc,
		                          ForwardDirection,
		                          NumPoints,
		                          0.f,
		                          MinDistFromSource,
		                          MaxDistFromSource,
		                          SpreadRadius,
		                          SpreadAxisRotation);
#pragma endregion

		// This creates the Leftmost Boundary for the Spread Radius
		const FVector LeftOfSpreadAngle = ForwardDirection.RotateAngleAxis(-SpreadRadius / 2.f, SpreadAxisRotation);

		// Adjust the spacing between points to avoid overlap at a 360-degree spread
		const float DeltaSpread = (SpreadRadius == 360.f) ? SpreadRadius / NumPoints : SpreadRadius / (NumPoints - 1);

		for (int32 i = 0; i < NumPoints; i++)
		{
			const FVector SpreadAngle = LeftOfSpreadAngle.RotateAngleAxis(DeltaSpread * i, SpreadAxisRotation);

			// Calculate Base Position without any offsets
			FVector BasePosition = (FixedDistFromSource > 0.f)
				? PointOriginLoc + SpreadAngle * FixedDistFromSource
				: PointOriginLoc + SpreadAngle * FMath::FRandRange(MinDistFromSource, MaxDistFromSource);

			// Offset Scale based on the position along the array, with a curve effect
			const float OffsetDistanceFromCenter = FMath::Abs(static_cast<float>(i) - (NumPoints - 1) / 2.f) / ((NumPoints - 1) / 2.f);
			const float OffsetScale = FMath::Square(OffsetDistanceFromCenter);

			// Apply Horizontal Offset based on SoftenFixedConeRadius amount and OffsetScale
			if (SoftenFixedConeRadius > 0.f) {
				// Horizontal Offset grows as points get farther from the center
				const FVector HorizontalOffset = SpreadAngle * (SoftenFixedConeRadius * OffsetScale);
				BasePosition += HorizontalOffset;
			}

			// Gyro Axis Point Placeholder
			FGyroAxisPoint GyroPt;
			GyroPt.Position = BasePosition;
			
			// Set Rotation based on the bUseDeltaAngleAsPtForward flag
			if (bUseDeltaAngleAsPtForward)
			{
				// Angles the Point Forward Direction to the same angle as the SpreadAxis it was generated on
				GyroPt.Rotation = FVector(-SpreadAngle.X, -SpreadAngle.Y, -SpreadAngle.Z + 90.f).Rotation();
			}
			else
			{
				// Angle the Point Forward Direction so that it faces straight up parallel with the Z axis
				GyroPt.Rotation = FRotator::ZeroRotator;
			}
			
			OutGyroRadialPts.Add(GyroPt);

			GyroAxisPointDebugHelpers(WorldContextObject,
			                          GyroPt,
			                          false,
			                          bDebugShowPointLocations,
			                          bDebugShowPointRotations);
		}
	}
	else
	{
		// Gyro Axis Point (Single) Placeholder
		FGyroAxisPoint GyroSinglePt;
		
		if (FixedDistFromSource > 0.f)
		{
			GyroSinglePt.Position = PointOriginLoc + ForwardDirection * FixedDistFromSource;
		}
		else
		{
			GyroSinglePt.Position =
				PointOriginLoc + ForwardDirection * FMath::FRandRange(MinDistFromSource, MaxDistFromSource);
		}

		// Set single point Rotation based on the bUseDeltaAngleAsPtForward flag
		if (bUseDeltaAngleAsPtForward)
		{
			// Angle the Point Forward Direction to the same angle as the SpreadAxis it was generated on
			GyroSinglePt.Rotation = (ForwardDirection + FVector(0.f ,0.f, -90.f)).Rotation();
		}
		else
		{
			// Angle the Point Forward Direction to the Up Vector (faces straight up parallel with the Z axis)
			GyroSinglePt.Rotation = FRotator::ZeroRotator;
		}

		OutGyroRadialPts.Add(GyroSinglePt);
		
		GyroAxisPointDebugHelpers(WorldContextObject,
								  GyroSinglePt,
								  false,
								  bDebugShowPointLocations,
								  bDebugShowPointRotations);
	}
}

void UAriaAbilitySystemLibrary::CreateGyroAxisPtSpiral(const UObject* WorldContextObject,
                                                       const FVector& PointOriginLoc,
                                                       const FVector& ForwardDirection,
                                                       const int32 NumPoints,
                                                       const float DistanceBetweenPoints,
                                                       const float DistanceBetweenSpirals,
                                                       const bool bReverseSpiralDirection,
                                                       const bool bDebugShowSourceToTargetDir,
                                                       const bool bDebugShowPointLocations,
                                                       const bool bDebugShowPointRotations,
                                                       TArray<FGyroAxisPoint>& OutGyroSpiralPts)
{
    // Ensure there is at least one point
    if (NumPoints <= 0)
    {
        return;
    }
    
    constexpr float AngleIncrementDeg = 10.0f; // Small angle increment
    float CurrentRadius = 0.0f;
    float CurrentAngleDeg = 0.0f;

    // Determine the angle increment direction based on the spiral direction
    const float AngleIncrementDir = (bReverseSpiralDirection ? -1 : 1) * AngleIncrementDeg;

    // Calculate the rotation matrix from the default up direction (Z-axis) to the ForwardDirection
    const FMatrix RotationMatrix = FRotationMatrix::MakeFromX(ForwardDirection);

    for (int32 i = 0; i < NumPoints; ++i)
    {
        const float AngleInRadians = FMath::DegreesToRadians(CurrentAngleDeg);
        FVector Offset = FVector(FMath::Cos(AngleInRadians) * CurrentRadius, FMath::Sin(AngleInRadians) * CurrentRadius, 0);

        // Apply rotation and translate to point origin location
        FVector RotatedOffset = RotationMatrix.TransformPosition(Offset);
        FVector NewPointPosition = PointOriginLoc + RotatedOffset;
        
        FRotator NewPointRotation = FRotator::ZeroRotator;
        FGyroAxisPoint GyroSpiralPt(NewPointPosition, NewPointRotation);
        OutGyroSpiralPts.Add(GyroSpiralPt);

        GyroAxisPointDebugHelpers(WorldContextObject,
                                  GyroSpiralPt,
                                  false,
                                  bDebugShowPointLocations,
                                  bDebugShowPointRotations);
        
        // Adjust the angle and radius based on DistanceBetweenPoints and DistanceBetweenSpirals
        CurrentRadius += DistanceBetweenPoints;
        CurrentAngleDeg += AngleIncrementDir * (DistanceBetweenPoints / DistanceBetweenSpirals);
    }

	// Shows Source Forward Direction
	GyroAxisPointDebugHelpers(WorldContextObject,
							  FGyroAxisPoint(),
							  false,
							  false,
							  false,
							  false,
							  bDebugShowSourceToTargetDir,
							  PointOriginLoc,
							  ForwardDirection);
}

void UAriaAbilitySystemLibrary::CreateGyroAxisPtCross(const UObject* WorldContextObject,
                                                      const FVector& PointOriginLoc,
                                                      const FVector& ForwardDirection,
                                                      const int32 NumPoints,
                                                      const float DistanceBetweenPoints,
                                                      const int32 TotalCrossDirections,
                                                      const float PointSrcDirectionalOffset,
                                                      const bool bDebugShowSourceToTargetDir,
                                                      const bool bDebugShowPointLocations,
                                                      const bool bDebugShowPointRotations,
                                                      TArray<FGyroAxisPoint>& OutGyroCrossPts)
{
	// Ensure there is at least one point
	if (NumPoints <= 0)
	{
		return;
	}
	
	// Adjust number of lines based on the total cross directions.
	const float AngleStep = 360.0f / TotalCrossDirections;

	// Creates each point based on the specified pattern
	for (int32 i = 0; i < TotalCrossDirections; ++i)
	{
		const float Angle = AngleStep * i;
		FVector Direction = ForwardDirection.RotateAngleAxis(Angle, FVector::UpVector);

		// Adjust the starting location by the directional offset amount
		FVector AdjustedOriginLocation = PointOriginLoc + Direction * PointSrcDirectionalOffset;
		
		TArray<FGyroAxisPoint> OutGyroXDirectionedPts;
		CreateGyroAxisPtDirectioned(WorldContextObject,
		                            AdjustedOriginLocation,
		                            Direction,
		                            NumPoints,
		                            DistanceBetweenPoints,
		                            bDebugShowSourceToTargetDir,
		                            bDebugShowPointLocations,
		                            bDebugShowPointRotations,
		                            OutGyroXDirectionedPts);
		OutGyroCrossPts.Append(OutGyroXDirectionedPts);
	}
}

void UAriaAbilitySystemLibrary::GenerateGyroPtSquareShape(const FVector& PointOriginLoc,
                                                          const FVector& ForwardDirection,
                                                          const float ShapeSize,
                                                          const int32 NumPoints,
                                                          TArray<FGyroAxisPoint>& OutGyroSquarePts)
{
    if (NumPoints < 4)
    {
        return;
    }

    const float BufferedShapeSize = ShapeSize + 100.f;
    const float HalfSide = BufferedShapeSize / 2.0f;

    TArray<FVector> SquareCorners;
    SquareCorners.Add(FVector(-HalfSide, -HalfSide, 0)); // Bottom-left
    SquareCorners.Add(FVector(HalfSide, -HalfSide, 0));  // Bottom-right
    SquareCorners.Add(FVector(HalfSide, HalfSide, 0));   // Top-right
    SquareCorners.Add(FVector(-HalfSide, HalfSide, 0));  // Top-left

    // Compute the rotation matrix
    const FMatrix RotationMatrix = FRotationMatrix::MakeFromX(ForwardDirection);

    // If we only have 4 points, place them at corners
    if (NumPoints == 4)
    {
        for (FVector Corner : SquareCorners)
        {
            Corner = RotationMatrix.TransformPosition(Corner) + PointOriginLoc;
            FRotator Rotation = FRotator::ZeroRotator;
            OutGyroSquarePts.Add(FGyroAxisPoint(Corner, Rotation));
        }
        return;
    }

    const int32 PointsPerEdge = (NumPoints - 4) / 4;
    const int32 ExtraPoints = (NumPoints - 4) % 4;

    for (int32 i = 0; i < 4; ++i)
    {
        FVector StartCorner = RotationMatrix.TransformPosition(SquareCorners[i]) + PointOriginLoc;
        FVector EndCorner = RotationMatrix.TransformPosition(SquareCorners[(i + 1) % 4]) + PointOriginLoc;
        FVector SegmentVector = (EndCorner - StartCorner).GetSafeNormal();
        const float SegmentLength = FVector::Distance(StartCorner, EndCorner);

        if (i != 0) 
        {
            FRotator VertexRotation = FRotator::ZeroRotator;
            OutGyroSquarePts.Add(FGyroAxisPoint(StartCorner, VertexRotation));
        }

        const int32 PointsToPlace = PointsPerEdge + (i < ExtraPoints ? 1 : 0);
        const float PointSpacing = SegmentLength / (PointsToPlace + 1);

        for (int32 j = 1; j <= PointsToPlace; ++j)
        {
            FVector NewPointPosition = StartCorner + SegmentVector * j * PointSpacing;
            FRotator NewPointRotation = FRotator::ZeroRotator;
            OutGyroSquarePts.Add(FGyroAxisPoint(NewPointPosition, NewPointRotation));
        }
    }

    // Finally, add the last corner manually to complete the loop
    const FVector LastVertex = RotationMatrix.TransformPosition(SquareCorners[0]) + PointOriginLoc;
    const FRotator LastVertexRotation = FRotator::ZeroRotator;
    OutGyroSquarePts.Add(FGyroAxisPoint(LastVertex, LastVertexRotation));
}

void UAriaAbilitySystemLibrary::GenerateGyroPtCircleShape(const FVector& PointOriginLoc,
														  const FVector& ForwardDirection,
														  const float ShapeSize,
														  const int32 NumPoints,
														  TArray<FGyroAxisPoint>& OutGyroCirclePts)
{
	if (NumPoints < 3)
	{
		return;
	}

	const float BufferedShapeSize = ShapeSize + 100.f;
	const float AngleIncrementDeg = 360.0f / NumPoints;

	// Compute the rotation matrix
	const FMatrix RotationMatrix = FRotationMatrix::MakeFromX(ForwardDirection);

	for (int32 i = 0; i < NumPoints; ++i)
	{
		const float AngleInRadians = FMath::DegreesToRadians(AngleIncrementDeg * i);
		FVector Offset = FVector(FMath::Cos(AngleInRadians) * BufferedShapeSize, FMath::Sin(AngleInRadians) * BufferedShapeSize, 0);

		// Apply rotation
		FVector RotatedOffset = RotationMatrix.TransformPosition(Offset);
		FVector PointPosition = PointOriginLoc + RotatedOffset;
		FRotator PointRotation = FRotator::ZeroRotator;
        
		OutGyroCirclePts.Add(FGyroAxisPoint(PointPosition, PointRotation));
	}
}

void UAriaAbilitySystemLibrary::GenerateGyroPtComplexShape(const FVector& PointOriginLoc,
                                                           const FVector& ForwardDirection,
                                                           int32 NumShapeVertices,
                                                           const float ShapeSize,
                                                           const int32 NumPoints,
                                                           TArray<FGyroAxisPoint>& OutGyroCompShapePts)
{
    // Ensure there are at least 3 points
    if (NumPoints < 3)
    {
        return;
    }

    // Adds a small buffer to the Shape Size
    const float BufferedShapeSize = ShapeSize + 100.f;

    // Adjust number of vertices if needed
    if (NumShapeVertices != 3)
    {
        NumShapeVertices = (NumShapeVertices % 2 == 0) ? NumShapeVertices : NumShapeVertices + 1;  
    }
    
    const float OuterRadius = BufferedShapeSize;
    const float InnerRadius = BufferedShapeSize * 0.5f;
    const float AngleIncrementDeg = 360.0f / NumShapeVertices;

    // Calculate the rotation matrix from the default north direction (X-axis) to the ForwardDirection
    const FMatrix RotationMatrix = FRotationMatrix::MakeFromX(ForwardDirection);

    TArray<FVector> ShapeVertices;
    float Radius;
    for (int32 i = 0; i < NumShapeVertices; ++i)
    {
        if (NumShapeVertices == 3)
        {
            // Triangle Shape
            Radius = OuterRadius;
        }
        else
        {
            // Other shapes (Star, Diamond)
            Radius = (i % 2 == 0) ? OuterRadius : InnerRadius;
        }

        const float AngleInRadians = FMath::DegreesToRadians(AngleIncrementDeg * i);
        FVector Offset = FVector(FMath::Cos(AngleInRadians) * Radius, FMath::Sin(AngleInRadians) * Radius, 0);
        
        // Apply rotation and translate to point origin location
        FVector RotatedOffset = RotationMatrix.TransformPosition(Offset);
        ShapeVertices.Add(PointOriginLoc + RotatedOffset);
    }

    // Determine the number of points per segment and the number of remaining points
    const int32 PointsPerSegment = FMath::Max((NumPoints / NumShapeVertices) - 1, 0); // Ensure no less than 0
    int32 RemainingPoints = NumPoints % NumShapeVertices;

    // Iterate over each segment and add points sequentially
    for (int32 i = 0; i < NumShapeVertices; ++i)
    {
        FVector StartVertex = ShapeVertices[i];
        FVector EndVertex = ShapeVertices[(i + 1) % NumShapeVertices];
        FVector SegmentVector = (EndVertex - StartVertex).GetSafeNormal();
        const float SegmentLength = FVector::Distance(StartVertex, EndVertex);

        // Add points at equal intervals along the edge
        const float DistancePerPoint = SegmentLength / (PointsPerSegment + 1);
        for (int32 j = 0; j <= PointsPerSegment; ++j) // Add points including start vertex
        {
            FVector NewPointPosition = StartVertex + SegmentVector * j * DistancePerPoint;
            //OutGyroCompShapePts.Add(FGyroAxisPoint(NewPointPosition, ForwardDirection.Rotation()));
        	OutGyroCompShapePts.Add(FGyroAxisPoint(NewPointPosition, FRotator::ZeroRotator));
        }

        // Distribute remaining points
        if (RemainingPoints > 0)
        {
            FVector ExtraPointPosition = StartVertex + SegmentVector * (PointsPerSegment + 1) * DistancePerPoint;
            //OutGyroCompShapePts.Add(FGyroAxisPoint(ExtraPointPosition, ForwardDirection.Rotation()));
        	OutGyroCompShapePts.Add(FGyroAxisPoint(ExtraPointPosition, FRotator::ZeroRotator));
            --RemainingPoints;
        }
    }
}

void UAriaAbilitySystemLibrary::GenerateGyroPtPentaStarShape(const FVector& PointOriginLoc,
                                                             const FVector& ForwardDirection,
                                                             int32 NumShapeVertices,
                                                             const float ShapeSize,
                                                             const int32 NumPoints,
                                                             TArray<FGyroAxisPoint>& OutGyroPentaPts)
{
    // Ensure there are at least 5 points
    if (NumPoints < 5)
    {
        return;
    }

    const float BufferedShapeSize = ShapeSize + 100.f;
    const float OuterRadius = BufferedShapeSize;

    TArray<FVector> StarVertices;

    // Calculate the rotation matrix from the default up direction (Z-axis) to the ForwardDirection
    const FMatrix RotationMatrix = FRotationMatrix::MakeFromX(ForwardDirection);

    // Calculate the vertex positions of the star
    for (int32 i = 0; i < NumShapeVertices; ++i)
    {
        constexpr float AngleIncrementDeg = 72.0f;
        const float AngleInRadians = FMath::DegreesToRadians(AngleIncrementDeg * i); // Regular pentagon vertices
        FVector Offset = FVector(FMath::Cos(AngleInRadians) * OuterRadius, FMath::Sin(AngleInRadians) * OuterRadius, 0);

        // Apply rotation
        FVector RotatedOffset = RotationMatrix.TransformPosition(Offset);
        StarVertices.Add(PointOriginLoc + RotatedOffset);
    }

    TArray<int32> ConnectionOrder = {0, 2, 4, 1, 3}; // The order to connect vertices to form a pentagram

    // Special case: If exactly 5 points, just place them at vertices
    if (NumPoints == 5)
    {
        for (const int32 i : ConnectionOrder)
        {
            const FVector& Vertex = StarVertices[i];
            FRotator PointRotation = FRotator::ZeroRotator;
            OutGyroPentaPts.Add(FGyroAxisPoint(Vertex, PointRotation));
        }
        return;
    }

    // General case: Distribute points along the edges
    const int32 PointsPerEdge = (NumPoints - NumShapeVertices) / NumShapeVertices;
    const int32 ExtraPoints = (NumPoints - NumShapeVertices) % NumShapeVertices;

    for (int32 i = 0; i < ConnectionOrder.Num(); ++i)
    {
        const FVector& StartVertex = StarVertices[ConnectionOrder[i]];
        const FVector& EndVertex = StarVertices[ConnectionOrder[(i + 1) % ConnectionOrder.Num()]];
        FVector SegmentVector = (EndVertex - StartVertex).GetSafeNormal();
        const float SegmentLength = FVector::Distance(StartVertex, EndVertex);

        // Start with the vertex point
        FRotator VertexRotation = FRotator::ZeroRotator;
        OutGyroPentaPts.Add(FGyroAxisPoint(StartVertex, VertexRotation));

        const int32 PointsToPlace = PointsPerEdge + (i < ExtraPoints ? 1 : 0);
        const float PointSpacing = SegmentLength / (PointsToPlace + 1);

        // Add points at equal intervals along the edge
        for (int32 j = 1; j <= PointsToPlace; ++j)
        {
            FVector NewPointPosition = StartVertex + SegmentVector * j * PointSpacing;
            FRotator NewPointRotation = FRotator::ZeroRotator;
            OutGyroPentaPts.Add(FGyroAxisPoint(NewPointPosition, NewPointRotation));
        }
    }
}

void UAriaAbilitySystemLibrary::CreateGyroAxisPtGrid(const UObject* WorldContextObject,
                                                     const FVector& PointOriginLoc,
                                                     const FVector& ForwardDirection,
                                                     const float DistanceBetweenPoints,
                                                     const int32 NumPoints,
                                                     const bool bDebugShowSourceToTargetDir,
                                                     const bool bDebugShowPointLocations,
                                                     const bool bDebugShowPointRotations,
                                                     TArray<FGyroAxisPoint>& OutGyroGridPts)
{
    // Ensure there are at least 9 points (3x3 grid)
    if (NumPoints < 9)
    {
        return;
    }

    // Add a buffer to the Distance Between Points
    const float DistBetweenPtsBuffer = DistanceBetweenPoints + 50;

    // Calculate grid dimensions
    int32 GridSize = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(NumPoints)));
    if (GridSize % 2 == 0)    // Ensure odd grid size for central origin point
    {
        GridSize += 1;
    }

    // Calculate the number of grid cells
    const int32 HalfGridSize = GridSize / 2;

    // Compute the rotation matrix from the default up direction (Z-axis) to the ForwardDirection
    const FMatrix RotationMatrix = FRotationMatrix::MakeFromX(ForwardDirection);

	// Shows Source Forward Direction
	GyroAxisPointDebugHelpers(WorldContextObject,
							  FGyroAxisPoint(),
							  false,
							  false,
							  false,
							  false,
							  bDebugShowSourceToTargetDir,
							  PointOriginLoc,
							  ForwardDirection);
	
    // Generate grid points, placing a point in the middle of each grid cell
    for (int32 i = -HalfGridSize; i <= HalfGridSize; ++i)
    {
        for (int32 j = -HalfGridSize; j <= HalfGridSize; ++j)
        {
            FVector Offset = FVector(i * DistBetweenPtsBuffer, j * DistBetweenPtsBuffer, 0);

            // Apply rotation
            FVector RotatedOffset = RotationMatrix.TransformPosition(Offset);
            FVector GridPointPos = PointOriginLoc + RotatedOffset;
            FRotator GridPointRot = FRotator::ZeroRotator;
            FGyroAxisPoint GyroGridPt(GridPointPos, GridPointRot);
            OutGyroGridPts.Add(GyroGridPt);

            GyroAxisPointDebugHelpers(WorldContextObject,
                                      GyroGridPt,
                                      false,
                                      bDebugShowPointLocations,
                                      bDebugShowPointRotations);
            
            // Stop once we have added the requested number of points
            if (OutGyroGridPts.Num() >= NumPoints)
            {
                return;
            }
        }
    }
}

void UAriaAbilitySystemLibrary::GyroAxisPointDebugHelpers(const UObject* WorldContextObject,
                                                          const FGyroAxisPoint& GyroAxisPt,
                                                          const bool bDebugShowPointGenDiameter,
                                                          const bool bDebugShowPointLocations,
                                                          const bool bDebugShowPointRotations,
                                                          const bool bDebugShowPointGenRadius,
                                                          const bool bDebugShowSourceToTargetDir,
                                                          const FVector& SourceLocation,
                                                          const FVector& SourceForwardVector,
                                                          const int32 NumPoints,
                                                          const float PointGenDiameter,
                                                          const float MinDistFromSource,
                                                          const float MaxDistFromSource,
                                                          const float SpreadRadius,
                                                          const FVector& SpreadAxisRotation)
{
	constexpr float DebugForwardDirLifetime = 8.6f;
	constexpr float DebugDiameterLifetime = 8.25f;
	constexpr float DebugRadiusLifetime = 8.25f;
	constexpr float DebugDeltaSpreadLifetime = 7.75f;
	constexpr float DebugShowPointLifetime = 8.85f;
	
	if (bDebugShowPointLocations)
	{
		// Visually displays a Debug Sphere exactly where the Point will be generated
		DrawDebugSphere(WorldContextObject->GetWorld(),
						GyroAxisPt.Position,
						18.f,
						12,
						FColor::Cyan,
						false,
						DebugShowPointLifetime);	
	}
	if (bDebugShowPointRotations)
	{
		// Visually displays the Rotation arrow pointer for each Point
		const FVector EndRotationLoc = GyroAxisPt.Position + GyroAxisPt.Rotation.RotateVector(FVector::UpVector) * 200.f;
		UKismetSystemLibrary::DrawDebugArrow(WorldContextObject,
		                                     GyroAxisPt.Position,
		                                     EndRotationLoc,
		                                     10.f,
		                                     FLinearColor::Red,
		                                     DebugShowPointLifetime,
		                                     8.f);	
	}
	if (bDebugShowSourceToTargetDir)
	{
		// Visually displays the Forward Direction from the Source location to the Target location 
		const FVector EndLocation = SourceLocation + SourceForwardVector * 150.f;
		UKismetSystemLibrary::DrawDebugArrow(WorldContextObject,
											 SourceLocation,
											 EndLocation,
											 15.f,
											 FLinearColor::Gray,
											 DebugForwardDirLifetime,
											 8.f);
	}
	if (bDebugShowPointGenDiameter)
	{
		// Visually displays a Debug Sphere for the diameter area
		DrawDebugSphere(WorldContextObject->GetWorld(),
						SourceLocation,
						PointGenDiameter,
						12,
						FColor::Silver,
						false,
						DebugDiameterLifetime);	
	}
	if (bDebugShowPointGenRadius)
	{
		const FVector LeftSpreadAngle = SourceForwardVector.RotateAngleAxis(-SpreadRadius / 2.f, SpreadAxisRotation);
		const FVector RightSpreadAngle = SourceForwardVector.RotateAngleAxis(SpreadRadius / 2.f, SpreadAxisRotation);
		const FVector MiddleSpreadAngle = (LeftSpreadAngle + RightSpreadAngle).GetSafeNormal();

		const float DeltaSpread = (SpreadRadius == 360.f) ? SpreadRadius / NumPoints : SpreadRadius / (NumPoints - 1);
			
		// Visually displays the Left and Right Boundaries of the Cone (AngleOfSpread)
		UKismetSystemLibrary::DrawDebugArrow(WorldContextObject, SourceLocation, SourceLocation + RightSpreadAngle * MaxDistFromSource, 4.f, FLinearColor::Green, DebugRadiusLifetime);
		UKismetSystemLibrary::DrawDebugArrow(WorldContextObject, SourceLocation, SourceLocation + LeftSpreadAngle * MaxDistFromSource, 4.f, FLinearColor::Blue, DebugRadiusLifetime);

		// Visually displays the Middle of the Cone (AngleOfSpread)
		UKismetSystemLibrary::DrawDebugArrow(WorldContextObject, SourceLocation, SourceLocation + MiddleSpreadAngle * (MaxDistFromSource + 75.f), 6.5f, FLinearColor::White, DebugRadiusLifetime, 5.f);
		
		// Visually displays the Beginning and Ending distance points for each Boundary of the Cone (AngleOfSpread)
		DrawDebugSphere(WorldContextObject->GetWorld(), SourceLocation + RightSpreadAngle * MinDistFromSource, 10.f, 12, FColor::Purple, false, DebugRadiusLifetime);
		DrawDebugSphere(WorldContextObject->GetWorld(), SourceLocation + RightSpreadAngle * MaxDistFromSource, 10.f, 12, FColor::Purple, false, DebugRadiusLifetime);
		DrawDebugSphere(WorldContextObject->GetWorld(), SourceLocation + LeftSpreadAngle * MinDistFromSource, 10.f, 12, FColor::Purple, false, DebugRadiusLifetime);
		DrawDebugSphere(WorldContextObject->GetWorld(), SourceLocation + LeftSpreadAngle * MaxDistFromSource, 10.f, 12, FColor::Purple, false, DebugRadiusLifetime);

		// Visually displays Arrows and Spheres to show the radial angles where each Point can be generated
		
		for (int32 i = 0; i < NumPoints; i++)
		{
			const FVector DeltaSpreadAngle = LeftSpreadAngle.RotateAngleAxis(DeltaSpread * i, SpreadAxisRotation);
			
			UKismetSystemLibrary::DrawDebugArrow(WorldContextObject, SourceLocation, SourceLocation + DeltaSpreadAngle * MaxDistFromSource, 4.f, FLinearColor::Yellow, DebugDeltaSpreadLifetime);
			DrawDebugSphere(WorldContextObject->GetWorld(), SourceLocation + DeltaSpreadAngle * MinDistFromSource, 5.f, 12, FColor::Orange, false, DebugDeltaSpreadLifetime);
			DrawDebugSphere(WorldContextObject->GetWorld(), SourceLocation + DeltaSpreadAngle * MaxDistFromSource, 5.f, 12, FColor::Orange, false, DebugDeltaSpreadLifetime);
		}
	}
}
#pragma endregion

TArray<FRotator> UAriaAbilitySystemLibrary::CreateRotatorsSpread(const UObject* WorldContextObject,
																 const FVector& SourceLocation,
																 const FVector& ForwardDirection,
																 const int32 RotationPoints,
																 const float SpreadRadius,
																 const FVector& RotationAxis,
																 const bool bDrawDebugRotationAngles)
{
	// Ensure there is at least one point
	if (RotationPoints <= 0)
	{
		return TArray<FRotator>();
	}
	
	if (!WorldContextObject) return TArray<FRotator>();
	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) TArray<FRotator>();
	
	TArray<FRotator> Rotators;

	const FVector LeftOfSpreadAngle = ForwardDirection.RotateAngleAxis(-SpreadRadius / 2.f, RotationAxis);

	if (RotationPoints > 1)
	{
		const float DeltaSpread = SpreadRadius / (RotationPoints - 1);
		for (int32 i = 0; i < RotationPoints; i++)
		{
			const FVector Direction = LeftOfSpreadAngle.RotateAngleAxis(DeltaSpread * i, RotationAxis);
			Rotators.Add(Direction.Rotation());

			/* GetRotationAngles_DebugHelpers */
#pragma region GetRotationAngles_DebugHelpers
			if (bDrawDebugRotationAngles)
			{
				// Debugging: Draws Arrows to show the radial angles where each Point is generated and angled towards
				UKismetSystemLibrary::DrawDebugArrow(World, SourceLocation, SourceLocation + Direction * 500, 4.f, FLinearColor::Yellow, 1.75f);
			}
#pragma endregion
		}	
	}
	else
	{
		Rotators.Add(ForwardDirection.Rotation());
	}
	return Rotators;
}

bool UAriaAbilitySystemLibrary::IsNotFriend(const AActor* FirstActor,
                                            const AActor* SecondActor)
{
	// Note: If FirstActor and SecondActor are the same, then this means they are likely on the same team (if not in PvP)

	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));

	const bool bFriends = bBothArePlayers || bBothAreEnemies;

	return !bFriends;
}

void UAriaAbilitySystemLibrary::ApplyDamageToTarget(const UAbilitySystemComponent* SourceAsc,
                                                    const AActor* DamageTarget,
                                                    const FGameplayTag& DamageType,
                                                    const float DamageAmount,
                                                    const int32 AbilityLevel,
                                                    const TSubclassOf<UGameplayEffect>& DamageEffectClass)
{
	// If no Target; return
	if (DamageTarget == nullptr || !IsValid(DamageTarget)) return;

	// If the Ability System Component of the AvatarActor itself is missing; return
	if (!IsValid(SourceAsc)) return;

	// If the Target Ability System Component is missing; return
	UAbilitySystemComponent* TargetAsc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DamageTarget);
	if (TargetAsc == nullptr || !IsValid(TargetAsc)) return;

	// Set the Base Damage Type and Amount
	const FGameplayEffectContextHandle DamageEffectContextHandle = SourceAsc->MakeEffectContext();

	const FGameplayEffectSpecHandle DamageSpecHandle = SourceAsc->MakeOutgoingSpec(
		DamageEffectClass, AbilityLevel, DamageEffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, DamageAmount);

	// Apply the Base Damage to Target Actor
	TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

FGameplayEffectContextHandle UAriaAbilitySystemLibrary::ApplyDamageEffect(const FAbilityModifier& DamageEffectParams)
{
	const FAriaGameplayTags& GameplayTags = FAriaGameplayTags::Get();

	const AActor* SourceAvatarActor = DamageEffectParams.SourceAsc->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	// KnockBack Effect
#pragma region KnockBack Effect
	//   Check whether Knockback should occur
	const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
	if (bKnockback)
	{
		// Sets up the Knockback effect
		FRotator ActorRotation = SourceAvatarActor->GetActorRotation();
		// This points the Actor 45 degrees upwards
		ActorRotation.Pitch = 45.f;
		// Get the Direction multiplied by the Force (Magnitude)
		const FVector KnockbackDirection = ActorRotation.Vector();
		const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;

		// Apply the Knockback Force to the Target (the actual Knockback effect)
		SetKnockbackForce(EffectContextHandle, KnockbackForce);
	}
#pragma endregion

	// Death Impulse Effect
#pragma region DeathImpulse Effect
	// Sets up the Death Impulse effect
	const FVector DeathImpulse = SourceAvatarActor->GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;

	// Apply the Death Impulse to the Target (the actual DeathImpulse effect)
	SetDeathImpulse(EffectContextHandle, DeathImpulse);
#pragma endregion
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAsc->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	// Applies the Set By Caller Magnitude for Base Damage for this Gameplay Effect
	if (DamageEffectParams.DamageType.IsValid() && DamageEffectParams.BaseDamage > 0.f)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType,
																	  DamageEffectParams.BaseDamage);
	}
	
	// Set the Status Effect Type
	if (DamageEffectParams.StatusEffectType.IsValid())
	{
		SetStatusEffectType(EffectContextHandle, DamageEffectParams.StatusEffectType);

		// Set the Damaged Attribute Type where applicable
		if (DamageEffectParams.DamagedAttributeType.IsValid())
		{
			SetDamagedAttributeType(EffectContextHandle, DamageEffectParams.DamagedAttributeType);	
		}
		
		// Applies the Set By Caller Magnitudes for the Status Effect-related params
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Chance,
																	  DamageEffectParams.StatusEffectChance);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Duration,
																	  DamageEffectParams.StatusEffectDurationMin);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Frequency,
																	  DamageEffectParams.StatusEffectFrequency);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Damage,
																	  DamageEffectParams.DamagedAttributeAmount);
	}
	
	DamageEffectParams.TargetAsc->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return EffectContextHandle;
}

int32 UAriaAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                             const ECharacterClass CharacterClass,
                                                             const float CharacterLevel)
{
	// Retrieve the Character Class Info data asset 
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	// Retrieve the Class Info (the info from the CharacterCLassInfo struct)
	const FCharacterClassData& ClassInfo = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);

	// Retrieve the correct XP Reward Amount for that Character's current level
	const float XPReward = ClassInfo.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

/* Attribute-related */
#pragma region Attributes
FGameplayAttribute UAriaAbilitySystemLibrary::GetAttributeByTag(const TMap<FGameplayTag,
																TStaticFuncPtr<FGameplayAttribute()>>& TagToAttributeContainer,
                                                                const FGameplayTag& AttributeTag)
{
	// Iterate through the map
	for (const auto& TagToAttribute : TagToAttributeContainer)
	{
		// Check if the current key matches the provided AttributeTag
		if (TagToAttribute.Key == AttributeTag)
		{
			// If match is found, call the function pointer to get and return the FGameplayAttribute
			return (*(TagToAttribute.Value))();
		}
	}

	// If no match is found, return a default FGameplayAttribute
	return FGameplayAttribute();
}
#pragma endregion
