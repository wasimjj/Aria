// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AriaAssetManager.generated.h"

/**
 * The UAriaAssetManager class is responsible for managing and loading assets in a game.
 *
 * These assets include visual effects, sounds, and other resources that are used to create
 * effects for characters or objects in the game.
 *
 * This class provides methods to load asset files, retrieve loaded assets, and manage the
 * memory used by those assets. It also handles the caching and recycling of assets to improve
 * performance and reduce resource consumption.
 *
 * The UAriaAssetManager is typically used in conjunction with the UAriaComponent class, which is
 * responsible for applying effects to game objects.
 *
 * Usage:
 *  1. Create an instance of UAriaAssetManager.
 *  2. Call the LoadAsset() method to load the asset files.
 *  3. Use the GetAsset() method to retrieve the loaded assets.
 *  4. Call the ReleaseAsset() method to release the assets when they are no longer needed.
 *  5. Dispose of the UAriaAssetManager instance when it is no longer needed.
 *
 * Example:
 * @code
 * UAriaAssetManager AriaAssetManager;
 *
 * // Load asset files
 * AriaAssetManager.LoadAsset("AriaAsset1");
 * AriaAssetManager.LoadAsset("AriaAsset2");
 *
 * // Retrieve the loaded assets
 * UAriaAsset Asset1 = AriaAssetManager.GetAsset("AriaAsset1");
 * UAriaAsset Asset2 = AriaAssetManager.GetAsset("AriaAsset2");
 *
 * // Use the assets
 * ...
 *
 * // Release the assets
 * AriaAssetManager.ReleaseAsset("AriaAsset1");
 * AriaAssetManager.ReleaseAsset("AriaAsset2");
 *
 * // Dispose of the AriaAssetManager instance
 * AriaAssetManager.Dispose();
 * @endcode
 */
UCLASS()
class ARIA_API UAriaAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAriaAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
