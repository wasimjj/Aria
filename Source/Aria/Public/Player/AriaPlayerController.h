// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "AriaPlayerController.generated.h"

class AAriaTargetingReticle;
class UInputMappingContext;
class UInputAction;
class ITargetingInterface;
class UAriaInputConfig;
class UNavigationSystemV1;
class UAriaAbilitySystemComponent;
class USplineComponent;
class UFloatingTextComponent;

struct FInputActionValue;

/**
 * The `AAriaPlayerController` class is a subclass of `APlayerController` that provides additional functionality
 * and customization for the game.
 *
 * It overrides certain methods to change default behavior and adds new methods for various features.
 *
 * It also declares several member variables and properties for different functionalities
 * and customization options, such as path-finding and navigation, input mapping (IMC), ability triggering,
 * point-and-click functionality, and more.
 *
 * @see APlayerController
 */
UCLASS()
class ARIA_API AAriaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAriaPlayerController();

	// Overriding the PlayerTick to change its default behavior
	virtual void PlayerTick(const float DeltaTime) override;

	// ShowDamageFloatingText gets passed to the Widget Component (ie. BP_FloatingDamageComponent) when spawned
	UFUNCTION(Client, Reliable)
	void ShowDamageFloatingText(const float DamageAmount, ACharacter* TargetCharacter, const bool bDodgedHit,
	                            const bool bBlockedHit, const bool bCriticalHit);

	// Function used for Showing/Hiding the AoETargetingReticle
	UFUNCTION(BlueprintCallable)
	void ShowAoETargetingReticle(UMaterialInterface* DecalMaterial = nullptr, const float AreaRadius = 200.f);
	UFUNCTION(BlueprintCallable)
	void HideAoETargetingReticle();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/* Point-and-Click (PAC) functionality - Click to Move */
#pragma region PointandClick_PAC_ClickToMove
	UPROPERTY()
	TObjectPtr<UNavigationSystemV1> NavSystem;

	UPROPERTY(EditDefaultsOnly, Category = "PAC Navigation")
	FVector QueryingExtentBounds = FVector(400.0f, 400.0f, 250.0f);
#pragma endregion

	/* Point-and-Click (PAC) functionality - Hold to Move */
#pragma region PointandClick_PAC_HoldToMove
	bool GetCursorPlaneIntersection(const FVector& InPlaneOrigin, const FVector& InPlaneNormal,
	                                FVector& OutPlanePoint) const;
	bool GetScreenPositionPlaneIntersection(const FVector2D& ScreenPosition, const FVector& InPlaneOrigin,
	                                        const FVector& InPlaneNormal, FVector& OutPlanePoint) const;
#pragma endregion

	/* Point-and-Click (PAC) functionality - Debug Helpers */
#pragma region PointandClick_PAC_DebugHelpers
	UPROPERTY(EditDefaultsOnly, Category = "PAC Navigation")
	bool bPAC_DrawDebugNavPtBoxes = false;

	UPROPERTY(EditDefaultsOnly, Category = "PAC Navigation")
	bool bPAC_DrawDebugPathLineArrows = false;
#pragma endregion

private:
	/* Input Mapping Context */
#pragma region Input Mapping Context
	// Note: Have to Forward Declare the UInputMappingContent > See the Class "UInputMappingContext" above.
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AriaContext;

	// Note: Have to Forward Declare the UInputAction > See the Class "UInputAction" above.
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	// The callback function used for handling Manual (W,A,S,D) Character movements.
	void OnMoveInputHeld(const FInputActionValue& InputActionValue);
#pragma endregion

	/* Shift (Special) Input Action */
#pragma region ShiftKey_InputAction
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	bool bShiftKeyDown = false;
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
#pragma endregion

	// This function handles the Cursor trace
	void CursorTrace();

	// Checks for any Hit Results
	FHitResult CursorHit;

	// Stores the Current and Previous Actors that were hit by the Cursor Trace.
	TScriptInterface<ITargetingInterface> LastActor;
	TScriptInterface<ITargetingInterface> ThisActor;

	/* Ability Input Handlers (Press, Hold, Release) */
#pragma region Ability_Input_Handlers
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAriaInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAriaAbilitySystemComponent> AriaAbilitySystemComponent;

	UAriaAbilitySystemComponent* GetAriaAsc();

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);
#pragma endregion

	/* Point-and-Click (PAC) functionality */
#pragma region PointandClick_PAC
	// Tracks how long the LMB has been held down before it was released
	//  This helps determine if it was a Short Press or not
	float FollowTime = 0.f;

	// Determines the Short Press Threshold;
	//  (i.e. how long do we hold the Mouse Cursor down before releasing it
	//   and how much time until we no longer consider that to be a Short Press)
	float ShortPressThreshold = 0.25f;

	// When Short Pressed; this tells whether or not to generate a Spline to have our Player Character follow
	//  We can call AddMovementInput every single frame as long as bAutoRunning remains true
	bool bAutoRunning = false;

	// This will determine the Radius at which point when reached; the Player Character can stop Autorunning
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	// This is the Spline Path that gets created from a series of World Locations during Click-to-Move movements
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> PathSpline;

	// Displays a visual location showing where the Mouse Click occurred
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickLocationNiagaraSystem;

	// Stores the ID for each Spline point that gets generated
	int32 TargetSplinePointIdx = 0;

	// Determines when to perform an Autorun
	bool bTargeting = false;

	// Causes the Player Character to Auto-run to their chosen Destination without requiring any movement input from the Player
	// Note: This is called from the Player Tick and is constantly ran every frame
	void AutoRun();
#pragma endregion

	// Used for displaying the Floating Damage Text that appears when hurt or Damaging an Enemy
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFloatingTextComponent> DamageTextComponentClass;

	// Used when Activating Abilities that require an AoE location indicator
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAriaTargetingReticle> AoETargetingReticleClass;
	UPROPERTY()
	TObjectPtr<AAriaTargetingReticle> AoETargetingReticle;

	void UpdateAoETargetingReticleLoc() const;
};
