// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ITargetingInterface
 *
 * @brief Interface class for actors that can be targeted and highlighted.
 *
 * The ITargetingInterface class provides two pure virtual functions to be implemented by actors that can be
 * targeted and highlighted in the game.
 * 
 * This interface is intended to be inherited by other classes to enable targeting functionality.
 *
 * @note This class is a pure abstract class, and any class that implements this interface will be
 * required to override these functions.
 */
class ARIA_API ITargetingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Setting "= 0" after the function makes it a 'Pure' class.
	//	These functions will not be able to use a Definition.
	//	These function are also considered "Abstract" and must be derived from.
	//		Any class that Implements this Interface will be required to override these functions.

	// Used for Displaying an Outline around the Actor being targeted by the Cursor
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;

	// Function used for Showing/Hiding the AoETargetingReticle
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowAoETargetingReticle(UMaterialInterface* DecalMaterial = nullptr, const float AreaRadius = 10.f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideAoETargetingReticle();
};
