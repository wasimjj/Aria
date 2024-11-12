// Copyright Savoie Interactive Studios


#include "Player/AriaPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AriaGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AriaAbilitySystemComponent.h"
#include "Actor/AriaTargetingReticle.h"
#include "Aria/Aria.h"
#include "Components/SplineComponent.h"
#include "Input/AriaInputComponent.h"
#include "Interaction/TargetingInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/FloatingTextComponent.h"
#include "Components/DecalComponent.h"

AAriaPlayerController::AAriaPlayerController()
{
	// Replication in Multiplayer Games; whenever an Entity changes on the Server,
	//	that change that took place on the Server will replicate (or be sent down) to all Clients
	//	connected to the Server.

	// This ensures that the Player Controller is designated to be a replicated Entity.
	bReplicates = true;

	/* Point-and-Click (PAC) functionality - (Path Spline Component Setup) */
#pragma region PointandClick_PAC_PathSpline
	// Constructs the Spine Component used in PathFinding
	//  Note: We will set its Spline Points as we generate Paths.
	PathSpline = CreateDefaultSubobject<USplineComponent>("PathSpline");
#pragma endregion
}

UAriaAbilitySystemComponent* AAriaPlayerController::GetAriaAsc()
{
	if (AriaAbilitySystemComponent == nullptr)
	{
		AriaAbilitySystemComponent = Cast<UAriaAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AriaAbilitySystemComponent;
}

void AAriaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//	Assert: Check will halt further execution if this condition fails (not having an AriaContext set)
	check(AriaContext);

	// Attaching to the Input Local Player Subsystem
	//	Subsystems are Singletons; they exist only for the duration of the Program.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	// Changed the Subsystem condition to an 'if' instead of a 'check' so that this will safely work for Multiplayer Games.
	// WARNING: Using a Subsystem assertion check for Multiplayer will cause the Game to crash.
	if (Subsystem)
	{
		// Note: The difference between using an If Statement and an Assertion (Check) is the Assertion will purposely crash the Program.

		// Adding the Input Mapping Context
		//	Note: You can have multiple Input Mapping Contexts at the same time, and it they are competing with each other for the same Inputs, then
		//	using Priority to determine which Input will win.
		//	When having a single Mapping Context; the Priority can just be set to 0.
		Subsystem->AddMappingContext(AriaContext, 0);
	}

	// Configuring the default Player Controller
	bShowMouseCursor = true; // <-- always displays the Mouse Cursor on screen.
	DefaultMouseCursor = EMouseCursor::Default;

	// Set the Default Input mode
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// <-- With this the Mouse cursor is not locked to the Viewport
	InputModeData.SetHideCursorDuringCapture(false);
	// <-- As soon as the Cursor is captured into the Viewport, the Game will not autohide it from view.
	SetInputMode(InputModeData);

	// Sets the Navigation System to use
	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
}

void AAriaPlayerController::PlayerTick(const float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	UpdateAoETargetingReticleLoc();
	
	/* Point-and-Click (PAC) functionality */
	//   Occurs after Click and Release (Short Press) to automatically move the Player Character to the selected location (via AutoRun function)
	AutoRun();
}

void AAriaPlayerController::ShowAoETargetingReticle(UMaterialInterface* DecalMaterial, const float AreaRadius)
{
	if (!IsValid(AoETargetingReticle))
	{
		AoETargetingReticle = GetWorld()->SpawnActor<AAriaTargetingReticle>(AoETargetingReticleClass);
		if (AreaRadius > 0.f)
		{
			// Uniformly set both Y and Z to AreaRadius to maintain a circular shape
			const FVector NewDecalSize = FVector(AoETargetingReticle->TargetingReticleDecal->DecalSize.X, AreaRadius, AreaRadius);
			AoETargetingReticle->TargetingReticleDecal->DecalSize = NewDecalSize;
				
			// Ensure rendered state is dirty so the change takes effect immediately
			AoETargetingReticle->TargetingReticleDecal->MarkRenderStateDirty();
		}
		if (DecalMaterial)
		{
			AoETargetingReticle->TargetingReticleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAriaPlayerController::HideAoETargetingReticle()
{
	if (IsValid(AoETargetingReticle))
	{
		AoETargetingReticle->Destroy();
	}
}

void AAriaPlayerController::UpdateAoETargetingReticleLoc() const
{
	if (IsValid(AoETargetingReticle))
	{
		AoETargetingReticle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

// This function involves getting the Hit Result under the Cursor.
//	The Player Controller has the inherent functionality to do this.
void AAriaPlayerController::CursorTrace()
{
	// Check whether Cursor Trace is allowed
	if (GetAriaAsc() && GetAriaAsc()->HasMatchingGameplayTag(FAriaGameplayTags::Get().Player_Block_CursorTrace))
	{
		// Unhighlight all Actors
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->UnHighlightActor();
		LastActor = nullptr;
		ThisActor = nullptr;

		return;
	}
	
	// Trace Complex is false because we only want to Trace against Simple Collision only.
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	/* Highlight Actor Feature */
#pragma region Highlight Actor Feature
	// Check if we have a Blocking Hit Result
	if (!CursorHit.bBlockingHit) return;

	// Setting for each Frame
	// This helps for the cases when the ThisActor is currently a null pointer reference
	LastActor = ThisActor;

	// Check if the Hit Result Actor implements the Targeting Interface
	//	If so, set the ThisActor to the result
	ThisActor = CursorHit.GetActor();

	// Set the Actor Highlight/UnHighlight
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
#pragma endregion
}

void AAriaPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	// Check whether to allow Pressed Inputs
	if (GetAriaAsc() && GetAriaAsc()->HasMatchingGameplayTag(FAriaGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());

	if (InputTag.MatchesTagExact(FAriaGameplayTags::Get().InputTag_LMB))
	{
		// Default Auto-running boolean to false at the start of every press
		//  (Will determine when to Autorun further below in the AbilityInputTagReleased function)
		bAutoRunning = false;
		// If targeting an Enemy or Object (ThisActor), set bTargeting true
		bTargeting = ThisActor ? true : false;
	}
	if (GetAriaAsc()) GetAriaAsc()->AscAbilityInputTagPressed(InputTag);
}

void AAriaPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	// Check whether to allow Held Inputs
	if (GetAriaAsc() && GetAriaAsc()->HasMatchingGameplayTag(FAriaGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	//GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());

	if (!InputTag.MatchesTagExact(FAriaGameplayTags::Get().InputTag_LMB))
	{
		// If another Input button is being held down that isn't the LMB, attempt to activate the associated Ability.
		if (GetAriaAsc()) GetAriaAsc()->AscAbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		// If currently Targeting an Actor or Object, attempt to activate the associated Ability.
		if (GetAriaAsc()) GetAriaAsc()->AscAbilityInputTagHeld(InputTag);
	}
	else
	{
		//  We ARE the LMB and are no longer targeting, then Player likely has clicked on an area that isn't an Actor or Object (..maybe they clicked somewhere on the Level to move to)

		/* Point-and-Click (PAC) functionality - (Hold to Move) */
#pragma region PointandClick_PAC_HoldToMove

		/* (This handles Click and HOLD to Move; i.e. holding the Mouse Button down to move
		 *  Note: This will only work when not targeting an Actor or Object.
		 */

		FollowTime += GetWorld()->GetDeltaSeconds();

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector PawnLocation = ControlledPawn->GetActorLocation();
			FVector CursorHorizPlaneIntersection;
			const bool bIntersectionFound = GetCursorPlaneIntersection(PawnLocation, FVector::UpVector,
			                                                           CursorHorizPlaneIntersection);
			if (bIntersectionFound)
			{
				FVector WorldDirection = (CursorHorizPlaneIntersection - PawnLocation).GetSafeNormal();
				WorldDirection.Z = 0.0f;
				ControlledPawn->AddMovementInput(WorldDirection);

				if (bPAC_DrawDebugPathLineArrows)
				{
					DrawDebugSphere(GetWorld(), CursorHorizPlaneIntersection, 20.0f, 12, FColor::Green);

					const FVector LineStart = PawnLocation + WorldDirection.GetSafeNormal() * 50.0f;
					const FVector LineEnd = LineStart + WorldDirection * 100.0f;
					UKismetSystemLibrary::DrawDebugArrow(this, LineStart, LineEnd, 20.0f, FLinearColor::Green, 0.0f, 4);
				}
			}
		}
#pragma endregion
	}
}

/* Point-and-Click (PAC) functionality - (Hold to Move) */
#pragma region PointandClick_PAC_HoldToMove
bool AAriaPlayerController::GetCursorPlaneIntersection(const FVector& InPlaneOrigin, const FVector& InPlaneNormal,
                                                       FVector& OutPlanePoint) const
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		const bool bMousePositionFound = LocalPlayer->ViewportClient->GetMousePosition(MousePosition);
		if (bMousePositionFound)
		{
			return GetScreenPositionPlaneIntersection(MousePosition, InPlaneOrigin, InPlaneNormal, OutPlanePoint);
		}
	}

	return false;
}

bool AAriaPlayerController::GetScreenPositionPlaneIntersection(const FVector2D& ScreenPosition,
                                                               const FVector& InPlaneOrigin,
                                                               const FVector& InPlaneNormal,
                                                               FVector& OutPlanePoint) const
{
	// Early out if we clicked on a HUD hitbox.
	const AHUD* HUD = GetHUD();
	if (HUD && HUD->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}

	FVector WorldOrigin;
	FVector WorldDirection;
	const bool bScreenPositionDeprojected = UGameplayStatics::DeprojectScreenToWorld(
		this, ScreenPosition, WorldOrigin, WorldDirection);
	if (bScreenPositionDeprojected)
	{
		OutPlanePoint = FMath::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance,
		                                             InPlaneOrigin, InPlaneNormal);
		return true;
	}

	return false;
}
#pragma endregion

void AAriaPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	// Check whether to allow Released Inputs
	if (GetAriaAsc() && GetAriaAsc()->HasMatchingGameplayTag(FAriaGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	//GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());

	if (!InputTag.MatchesTagExact(FAriaGameplayTags::Get().InputTag_LMB))
	{
		// If another Input button is released that isn't the LMB, attempt to deactivate the associated Ability.
		if (GetAriaAsc()) GetAriaAsc()->AscAbilityInputTagReleased(InputTag);
		return;
	}

	// If currently Targeting an Actor or Object, attempt to release the associated Ability.
	if (GetAriaAsc()) GetAriaAsc()->AscAbilityInputTagReleased(InputTag);

	if (!bTargeting & !bShiftKeyDown)
	{
		//  We ARE the LMB and are no longer targeting, then Player likely has clicked on an area that isn't an Actor or Object (..maybe they clicked somewhere on the Level to move to)

		/* Point-and-Click (PAC) functionality - (AutoMove) */
#pragma region PointandClick_PAC_AutoMove

		/* (This handles Click and Release (Short Press) to Move
		 *  Note: This will only work when not targeting an Actor or Object.
		 */

		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			FHitResult NavChannelCursorHitResult;
			GetHitResultUnderCursor(ECC_PAC_Navigation, false, NavChannelCursorHitResult);
			if (NavChannelCursorHitResult.bBlockingHit)
			{
				// Projecting a point from the cursor impact point to the NavMesh with a larger-than-default
				// Query Extent, so there are better chances to reach for the NavMesh and return a point,
				// and then generating a path from the pawn location to this point (only if found).

				FNavLocation ImpactPointNavLocation;
				// NOTE: Default Query Extend = FVector(50.0f, 50.0f, 250.0f)
				const FNavAgentProperties& NavAgentProps = GetNavAgentPropertiesRef();
				const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(
					NavChannelCursorHitResult.ImpactPoint, ImpactPointNavLocation, QueryingExtentBounds,
					&NavAgentProps);
				if (bNavLocationFound)
				{
					UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
						this, ControlledPawn->GetActorLocation(), ImpactPointNavLocation);
					if (NavigationPath && NavigationPath->PathPoints.Num() > 0)
					{
						PathSpline->ClearSplinePoints();
						for (const FVector& PathPoint : NavigationPath->PathPoints)
						{
							PathSpline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
						}
						TargetSplinePointIdx = 1;
						bAutoRunning = true;
					}
				}

				if (bPAC_DrawDebugNavPtBoxes)
				{
					DrawDebugBox(GetWorld(), NavChannelCursorHitResult.ImpactPoint, QueryingExtentBounds,
					             FColor::Silver, false, 3.0f);
				}
			}
			
			// Display the Mouse Click Location (only if Pressed Input is allowed)
			if (GetAriaAsc() && !GetAriaAsc()->HasMatchingGameplayTag(FAriaGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickLocationNiagaraSystem, NavChannelCursorHitResult.ImpactPoint);
			}
		}
		FollowTime = 0.0f;
		bTargeting = false;
#pragma endregion
	}
}

/* Point-and-Click (PAC) functionality - (Autorun) */
#pragma region PointandClick_PAC_Autorun
void AAriaPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector PawnLocation = ControlledPawn->GetActorLocation();

		// This is the Location on the Spline Path that is closest to the Player Character
		const FVector LocationOnPathSpline = PathSpline->FindLocationClosestToWorldLocation(
			PawnLocation, ESplineCoordinateSpace::World);

		// This is the Location of the 'next' Spline Point that the Character will move towards
		const FVector TargetSplinePointLocation = PathSpline->GetLocationAtSplinePoint(
			TargetSplinePointIdx, ESplineCoordinateSpace::World);

		// Get the Direction on the Spline Path from the Character facing position to that Spline Point
		FVector WorldDirection = TargetSplinePointLocation - PawnLocation;
		WorldDirection.Z = 0.0f;

		// NOTE: We get the normal after zeroing Z to get a constant movement speed along the XY plane.
		WorldDirection = WorldDirection.GetSafeNormal();

		// Move the Player Character along the Path Spline
		ControlledPawn->AddMovementInput(WorldDirection);

		const float DistanceToTarget = (LocationOnPathSpline - TargetSplinePointLocation).Length();
		if (DistanceToTarget <= AutoRunAcceptanceRadius)
		{
			// Note on AutoRunAcceptanceRadius: If the Player Character ever gets stuck while trying to reach a PathSpline point, the AutoRunAcceptanceRadius value can be increased to fix the problem

			const bool bNextTargetPointExist = TargetSplinePointIdx < PathSpline->GetNumberOfSplinePoints() - 1;
			if (bNextTargetPointExist)
			{
				TargetSplinePointIdx++;
			}
			else
			{
				// If close to or at our Destination; stop Auto-running
				bAutoRunning = false;
			}
		}

		// DEBUGGING
		if (bPAC_DrawDebugPathLineArrows)
		{
			for (int32 SplinePointIdx = 0; SplinePointIdx < PathSpline->GetNumberOfSplinePoints(); ++SplinePointIdx)
			{
				const FVector SplinePointLocation = PathSpline->GetLocationAtSplinePoint(
					SplinePointIdx, ESplineCoordinateSpace::World);
				if (SplinePointIdx > 0)
				{
					const FVector PreviousSplinePointLocation = PathSpline->GetLocationAtSplinePoint(
						SplinePointIdx - 1, ESplineCoordinateSpace::World);
					DrawDebugLine(GetWorld(), PreviousSplinePointLocation, SplinePointLocation, FColor::Red);
				}
				DrawDebugSphere(GetWorld(), SplinePointLocation, 10.0f, 12, FColor::Red);
			}
			DrawDebugSphere(GetWorld(), LocationOnPathSpline, 20.0f, 12, FColor::Cyan);

			const FVector LineStart = PawnLocation + WorldDirection.GetSafeNormal() * 50.0f;
			const FVector LineEnd = LineStart + WorldDirection * 100.0f;
			UKismetSystemLibrary::DrawDebugArrow(this, LineStart, LineEnd, 20.0f, FLinearColor::Yellow, 0.0f, 4);

			DrawDebugSphere(GetWorld(), TargetSplinePointLocation, 20.0f, 12, FColor::Yellow);

			UE_LOG(LogTemp, Warning, TEXT("TargetSplinePointIdx: %i, DistanceToTarget: %f"), TargetSplinePointIdx,
			       DistanceToTarget);
		}
	}
}
#pragma endregion

// Handles the Player Controller Actions such as Movement, etc.
void AAriaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//	With CastChecked, if the Cast fails, it acts the same as using an Assert and will crash the Program.
	UAriaInputComponent* AriaInputComponent = CastChecked<UAriaInputComponent>(InputComponent);

	// Bind the Move Action to the Movement Action (IA_Move)
	AriaInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAriaPlayerController::OnMoveInputHeld);

	// Bindings for the Shift special Action
	AriaInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAriaPlayerController::ShiftPressed);
	AriaInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAriaPlayerController::ShiftReleased);

	// When we press any of the Inputs in the Input Config Data Asset, then the three callback functions (Pressed, Released, Held) will be triggered
	AriaInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

// Handled Manual Player Movements (Through W,A,S,D keys)
void AAriaPlayerController::OnMoveInputHeld(const FInputActionValue& InputActionValue)
{
	// Check whether to allow Movement
	if (GetAriaAsc() && GetAriaAsc()->HasMatchingGameplayTag(FAriaGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	// Sets the Input Action value in the form of a 2D axis vector
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// Gets the Rotation of the Controller
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRRotation(0.f, Rotation.Yaw, 0.f);

	// Check the Forward Vector (Is the Character moving Forward)
	const FVector ForwardDirection = FRotationMatrix(YawRRotation).GetUnitAxis(EAxis::X);

	// Check the Right Vector (Is the Character moving Right)
	const FVector RightDirection = FRotationMatrix(YawRRotation).GetUnitAxis(EAxis::Y);

	// Apply Movement for the Character in the appropriate direction based on the InputAxisVector (1 = Positive movement, -1 = Negative movement)
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

	// Added for overriding any Navigational Spline Pathing that might be active
	bAutoRunning = false;
}

void AAriaPlayerController::ShowDamageFloatingText_Implementation(const float DamageAmount, ACharacter* TargetCharacter,
                                                                  const bool bDodgedHit, const bool bBlockedHit,
                                                                  const bool bCriticalHit)
{
	// This will Construct the Damage Widget Component and call the SetNumberFloatingText once constructed
	//  With "IsLocalController()", we'll only see that Damage Floating Text if we're the Local Controller, The Server should not need to see these floating text numbers
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UFloatingTextComponent* DamageText = NewObject<UFloatingTextComponent>(
			TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),
		                              FAttachmentTransformRules::KeepRelativeTransform);

		// Set the Widget Component position (Based on center of attachment point ((i.e. root center)
		FTransform InitialTransform = FTransform();
		InitialTransform.SetTranslation(FVector(0.f, 0.f, 130.f));
		DamageText->SetRelativeTransform(InitialTransform);

		// Immediately Detaching once spawned so that the Floating Text will not follow the character around after it has been activated
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageText->SetDamageFloatingText(DamageAmount, bDodgedHit, bBlockedHit, bCriticalHit);
	}
}
