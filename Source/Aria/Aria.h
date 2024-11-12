// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#pragma region Custom Macros

// Used for our Target Highlighting color
#define CUSTOM_DEPTH_RED 250

// Our Projectile Object Collision Channel; Created in Project Settings in GameTrace Channel index position 1
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

// Our Point-and-Click (PAC) Navigation Channel; Created in Project Settings in GameTrace Channel index position 2
#define ECC_PAC_Navigation ECollisionChannel::ECC_GameTraceChannel2

#pragma endregion

struct FGraphPanelPinFactory;

// Used with the Curve Table Curve Selector 
class FAriaModule final : public IModuleInterface
{
public:

};
