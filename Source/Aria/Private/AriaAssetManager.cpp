// Copyright Savoie Interactive Studios


#include "AriaAssetManager.h"
#include "AriaGameplayTags.h"
// #include "AbilitySystemGlobals.h"

UAriaAssetManager& UAriaAssetManager::Get()
{
	check(GEngine);

	UAriaAssetManager* AriaAssetManager = Cast<UAriaAssetManager>(GEngine->AssetManager);
	return *AriaAssetManager;
}

void UAriaAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//Initialize native Gameplay Tags for our Project
	FAriaGameplayTags::InitializeNativeGameplayTags();

	// Initializes certain features needed by the Game Project; such as TargetData
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
