// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "AriaAbilityTypes.h"
#include "AriaAttributeSet.h"
#include "Logging/LogMacros.h"
#include "GameplayEffectTypes.h"
#include "Aria/AriaLogChannels.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "Data/AbilityInfo.h"
#include "AriaAbilitySystemLibrary.generated.h"

class UAriaGameplayAbility;
class UAbilitySystemComponent;
class AAriaHUD;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilityMenuWidgetController;

struct FWidgetControllerParams;

USTRUCT(BlueprintType)
struct FGyroAxisPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Point")
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation")
	FRotator Rotation;

	FGyroAxisPoint()
		: Position(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {}

	FGyroAxisPoint(const FVector& InPosition, const FRotator& InRotation)
		: Position(InPosition), Rotation(InRotation) {}

	static FRotator CalculateRotationFromNormal(const FVector& Normal)
	{
		return FRotationMatrix::MakeFromZ(Normal).Rotator();
	}

	// Defines the comparison operator < ; used in custom sorting algorithms
	bool operator<(const FGyroAxisPoint& Other) const
	{
		return Position.Size() < Other.Position.Size();
	}
};

enum class ECharacterClass : uint8;

/**
 * \class UAriaAbilitySystemLibrary
 * \brief The UAriaAbilitySystemLibrary class provides a set of static functions that can be used to interact
 * with the Aria Ability System.
 *
 * This class is a blueprint function library and inherits from UBlueprintFunctionLibrary.
 *
 * \note The Aria Ability System is a game system that manages abilities, attributes, and gameplay effects for characters.
 */
UCLASS()
class ARIA_API UAriaAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* UI-related */
#pragma region UI
	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|WidgetController",
		meta = (DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,
	                                       FWidgetControllerParams& OutWcParams,
	                                       AAriaHUD*& OutAriaHud);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|WidgetController",
		meta = (DefaultToSelf="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|WidgetController",
		meta = (DefaultToSelf="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|WidgetController",
		meta = (DefaultToSelf="WorldContextObject"))
	static UAbilityMenuWidgetController* GetAbilityMenuWidgetController(const UObject* WorldContextObject);
#pragma endregion

	// This function is used for initializing Attributes based on the Character Job Class and Level
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,
	                                        ECharacterClass CharacterClass,
	                                        float CharacterLevel,
	                                        UAbilitySystemComponent* AbilitySystemComponent);

	// Initialize Common Gameplay Abilities
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject,
	                                 UAbilitySystemComponent* AbilitySystemComponent,
	                                 ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|CharacterClassDefaults")
	static TArray<TSubclassOf<UGameplayAbility>> GetCharacterClassAbilities(const UObject* WorldContextObject,
	                                                                        const ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	// Note: UPARAM(ref) means that the Handle will be treated as an Input pin in blueprints instead of an Output pin.

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetIsDodgedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                           const bool bInIsDodgedHit);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                            const bool bInIsBlockedHit);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                             const bool bInIsCriticalHit);

	/* Ability Modifiers */
#pragma region AbilityModifiers

	/* Damage */

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                          const FGameplayTag& InDamageType);

	/* Knockbacks */

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                              const FVector& InKnockbackForce);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                            const FVector& InDeathImpulse);

	/* Status Effect-related */
#pragma region StatusEffects
	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsStatusEffectApplied(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetStatusEffectApplied(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                   const bool bInStatusEffectApplied);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetStatusEffectType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetStatusEffectType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                const FGameplayTag& InStatusEffectType);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetStatusEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetStatusEffectDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                    const float InStatusEffectDuration);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetStatusEffectFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetStatusEffectFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                     const float InStatusEffectFrequency);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamagedAttributeType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetDamagedAttributeType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                    const FGameplayTag& InDamagedAttributeType);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetDamagedAttributeAmount(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetDamagedAttributeAmount(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                      const float InDamagedAttributeAmount);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static EAdditionalDamageTriggers
	GetAdditionalDamageTrigger(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetAdditionalDamageTrigger(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                       const EAdditionalDamageTriggers InAdditionalDamageTrigger);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetAdditionalDamageAmount(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetAdditionalDamageAmount(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                      const float InAdditionalDamageAmount);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static ERandomlyMissAnyActions GetRandomlyMissActions(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetRandomlyMissActions(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                   const ERandomlyMissAnyActions InRandomlyMissActions);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetRandomMissPercentage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetRandomMissPercentage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                    const float InRandomMissPercentage);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static ERandomlyStopAnyActions GetRandomlyStopActions(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetRandomlyStopActions(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                   const ERandomlyStopAnyActions InRandomlyStopActions);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetReducedActionPercentage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetReducedActionPercentage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                       const float InReducedActionPercentage);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static EStopAllActions GetStopAllActions(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetStopAllActions(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                              const EStopAllActions InStopAllActions);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static float GetReducedMovementSpeed(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetReducedMovementSpeed(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                    const float InReducedMovementSpeed);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsRandomizeMovements(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetRandomizeMovements(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                  const bool bInRandomizeMovements);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsRunFromTarget(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetRunFromTarget(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                             const bool bInRunFromTarget);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsInstantKillTarget(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetInstantKillTarget(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                 const bool bInInstantKillTarget);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static int32 GetReducedLevels(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetReducedLevels(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                             const int32 InReducedLevels);

	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static bool IsForceAIControl(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayEffects")
	static void SetForceAIControl(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                              const bool bInForceAIControl);
#pragma endregion

#pragma endregion
	
	// This can be used for different things; such as getting all of the Live Players within an Attack Radius
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveActorsWithinRadius(const UObject* WorldContextObject,
										  TArray<AActor*>& OutOverlappingActors,
	                                      const TArray<AActor*>& ActorsToIgnore,
	                                      const float Radius,
	                                      const FVector& SphereOriginLoc,
	                                      const bool bDrawDebuggingSphere = false,
	                                      const float DebugSphereLifetime = 1.f);

	// Returns the Closest # (<- adjustable via MaxActors) of Actors within the Actor Radius
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestActors(int32 MaxActors,
								 const TArray<AActor*>& ActorsWithinRadius,
								 TArray<AActor*>& OutClosestActors,
								 const FVector& Origin);

	// Gets the Actors surrounding a Target 
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics")
	static TArray<AActor*> GetActorsAroundTarget(const int32 MaxSurroundingActors = 3,
												 const float Radius = 150.f,
		                                         const AActor* SourceActor = nullptr,
		                                         const AActor* TargetActor = nullptr,
		                                         const FVector TargetLocation = FVector::ZeroVector,
		                                         const bool bIncludeSourceActor = false,
		                                         const bool bIncludeTargetActor = false,
		                                         const bool bDrawDebugSpheres = false);

	// Function used to return a Point aligned to the Surface Normal of where the Line Trace collided with that surface
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics")
	static void GetGroundSurfacePoint(const UObject* WorldContextObject,
	                                  const FGyroAxisPoint& GyroPoint,
	                                  const float MinDistAboveSurface,
	                                  const float MaxDistAboveSurface,
	                                  const float MinDistBelowSurface,
	                                  const float MaxDistBelowSurface,
	                                  const TArray<AActor*>& ActorsToIgnore,
	                                  const bool bApplyPointNormalRotations,
	                                  const float NormalRangeTolerance,
	                                  const bool bDrawDebugLineTrace,
	                                  FGyroAxisPoint& OutGyroSurfacePt);

	/* GyroAxisPoint Generate Patterns */
#pragma region GyroAxisPt_GenPatterns
	// Generates a single GyroAxisPoint at the specified location
	static void CreateGyroAxisPtSingle(const UObject* WorldContextObject,
	                                   const FVector& PointOriginLoc,
	                                   const bool bDebugShowPointLocations,
	                                   const bool bDebugShowPointRotations,
	                                   FGyroAxisPoint& OutGyroSinglePt);
	
	// Generates an array of GyroAxisPoints in a straight line
	static void CreateGyroAxisPtDirectioned(const UObject* WorldContextObject,
	                                        const FVector& PointOriginLoc,
	                                        const FVector& ForwardDirection,
	                                        const int32 NumPoints,
	                                        const float DistanceBetweenPoints,
	                                        const bool bDebugShowSourceToTargetDir,
	                                        const bool bDebugShowPointLocations,
	                                        const bool bDebugShowPointRotations,
	                                        TArray<FGyroAxisPoint>& OutGyroDirectionedPts);
	
	// Generates one or more (evenly-spaced) GyroAxisPoints at a Source Location based on a provided Radius and # of Points
	//  Note: Useful for creating Projectile spreads and similar features that require a Spread Algorithm
	// Scenario 1: Using a smaller SpreadRadius amount creates a wide cone that appears similar to waves like Contra's Spreader gun or Diablo's Multi-shot Arrows (<- Especially after applying a SoftenFixedConeRadius amount of 50 or so)
	// Scenario 2: Using a larger SpreadRadius can create a wider  cone spread and a 360 degree Spread Radius will create a full circular spread (<- useful for spawning points around the player, or friendly / enemy target)
	//  Formula Example: A SpreadRadius of 90 and a Spread Amount (PointsToGenerate) of 5 would give 5 Directions spread across 19 degree intervals
	static void CreateGyroAxisPtSpread(const UObject* WorldContextObject,
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
	                                   TArray<FGyroAxisPoint>& OutGyroRadialPts);

	static void CreateGyroAxisPtSpiral(const UObject* WorldContextObject,
	                                   const FVector& PointOriginLoc,
	                                   const FVector& ForwardDirection,
	                                   const int32 NumPoints,
	                                   const float DistanceBetweenPoints,
	                                   const float DistanceBetweenSpirals,
	                                   const bool bReverseSpiralDirection,
	                                   const bool bDebugShowSourceToTargetDir,
	                                   const bool bDebugShowPointLocations,
	                                   const bool bDebugShowPointRotations,
	                                   TArray<FGyroAxisPoint>& OutGyroSpiralPts);

	static void CreateGyroAxisPtCross(const UObject* WorldContextObject,
	                                  const FVector& PointOriginLoc,
	                                  const FVector& ForwardDirection,
	                                  const int32 NumPoints,
	                                  const float DistanceBetweenPoints,
	                                  const int32 TotalCrossDirections,
	                                  const float PointSrcDirectionalOffset,
	                                  const bool bDebugShowSourceToTargetDir,
	                                  const bool bDebugShowPointLocations,
	                                  const bool bDebugShowPointRotations,
	                                  TArray<FGyroAxisPoint>& OutGyroCrossPts);

	static void GenerateGyroPtSquareShape(const FVector& PointOriginLoc,
	                                      const FVector& ForwardDirection,
	                                      const float ShapeSize,
	                                      const int32 NumPoints,
	                                      TArray<FGyroAxisPoint>& OutGyroSquarePts);

	static void GenerateGyroPtCircleShape(const FVector& PointOriginLoc,
	                                      const FVector& ForwardDirection,
	                                      const float ShapeSize,
	                                      const int32 NumPoints,
	                                      TArray<FGyroAxisPoint>& OutGyroCirclePts);

	static void GenerateGyroPtComplexShape(const FVector& PointOriginLoc,
	                                       const FVector& ForwardDirection,
	                                       int32 NumShapeVertices,
	                                       const float ShapeSize,
	                                       const int32 NumPoints,
	                                       TArray<FGyroAxisPoint>& OutGyroCompShapePts);

	static void GenerateGyroPtPentaStarShape(const FVector& PointOriginLoc,
	                                         const FVector& ForwardDirection,
	                                         int32 NumShapeVertices,
	                                         const float ShapeSize,
	                                         const int32 NumPoints,
	                                         TArray<FGyroAxisPoint>& OutGyroPentaPts);

	static void CreateGyroAxisPtGrid(const UObject* WorldContextObject,
	                                 const FVector& PointOriginLoc,
	                                 const FVector& ForwardDirection,
	                                 const float DistanceBetweenPoints,
	                                 const int32 NumPoints,
	                                 const bool bDebugShowSourceToTargetDir,
	                                 const bool bDebugShowPointLocations,
	                                 const bool bDebugShowPointRotations,
	                                 TArray<FGyroAxisPoint>& OutGyroGridPts);

	static void GyroAxisPointDebugHelpers(const UObject* WorldContextObject,
	                                      const FGyroAxisPoint& GyroAxisPt,
	                                      const bool bDebugShowPointGenDiameter = false,
	                                      const bool bDebugShowPointLocations = false,
	                                      const bool bDebugShowPointRotations = false,
	                                      const bool bDebugShowPointGenRadius = false,
	                                      const bool bDebugShowSourceToTargetDir = false,
	                                      const FVector& SourceLocation = FVector::ZeroVector,
	                                      const FVector& SourceForwardVector = FVector::ZeroVector,
	                                      const int32 NumPoints = 0,
	                                      const float PointGenDiameter = 0.f,
	                                      const float MinDistFromSource = 0.f,
	                                      const float MaxDistFromSource = 0.f,
	                                      const float SpreadRadius = 0.f,
	                                      const FVector& SpreadAxisRotation = FVector::UpVector);
#pragma endregion

	// Function used for generating a series of Rotators based on a specified radius
	//  These points are generated from a single location and spread out at various angles based on the point count
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> CreateRotatorsSpread(const UObject* WorldContextObject,
												 const FVector& SourceLocation,
												 const FVector& ForwardDirection,
												 const int32 RotationPoints = 5,
												 const float SpreadRadius = 145.f,
												 const FVector& RotationAxis = FVector::UpVector,
												 const bool bDrawDebugRotationAngles = false);
	
	// Check whether or not the Actor(s) are friendly to you (Helps prevent Friendly Fire scenarios)
	UFUNCTION(BlueprintPure, Category="AriaAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor,
	                        const AActor* SecondActor);

	// Used for applying Base Damage from a Damage Type without any additional Ability Modifiers
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics|Damage")
	static void ApplyDamageToTarget(const UAbilitySystemComponent* SourceAsc,
	                                const AActor* DamageTarget,
	                                const FGameplayTag& DamageType,
	                                const float DamageAmount,
	                                const int32 AbilityLevel,
	                                const TSubclassOf<UGameplayEffect>& DamageEffectClass);

	// Creates a Damage Gameplay Effect that can be applied based on the Damage Effect params
	UFUNCTION(BlueprintCallable, Category="AriaAbilitySystemLibrary|GameplayMechanics|Damage")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FAbilityModifier& DamageEffectParams);
	
	// Function used for looking up and returning the correct XP Amount based on the Character Job Class and Level; includes rounding calculation
	//  Note: Needed a WorldContextObject since we need to use a GameplayStatics function to get access to the GameMode where the CharacterClassInfo data lives
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	                                         const ECharacterClass CharacterClass,
	                                         const float CharacterLevel);

	/* Attribute-related */
#pragma region Attributes
	static FGameplayAttribute GetAttributeByTag(
		const TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& TagToAttributeContainer,
		const FGameplayTag& AttributeTag);
#pragma endregion

	/* Template Functions */
#pragma region TemplateFunctions
	// Generic function used to find a Row from the DataTable where matches the Tag.
	template <typename T>
	static T* GetDataTableRowByTag(UDataTable* DataTable,
	                               const FGameplayTag& Tag);

	template <typename T>
	static T* GetDataTableRowByTagAndRowIndex(UDataTable* DataTable,
	                                          const FGameplayTag& Tag,
	                                          int32 RowIndex);

	template <typename T>
	static T* GetAbilityDescriptionsRow(UDataTable* DataTable,
	                                    const FGameplayTag& AbilityTag,
	                                    int32 AbilityLevel);
#pragma endregion
};

/* Template Functions - Implementations (Important: These MUST stay in the Header where they are defined) */
#pragma region TemplateFunctionImplementations
template <typename T>
T* UAriaAbilitySystemLibrary::GetDataTableRowByTag(UDataTable* DataTable,
                                                   const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}

template <typename T>
T* UAriaAbilitySystemLibrary::GetDataTableRowByTagAndRowIndex(UDataTable* DataTable,
                                                              const FGameplayTag& Tag,
                                                              int32 RowIndex)
{
	if (!DataTable)
	{
		UE_LOG(LogAria, Warning, TEXT("DataTable is nullptr"));
		return nullptr;
	}

	// Get all row names in the data table
	const TArray<FName> RowNames = DataTable->GetRowNames();

	// Ensure the row index is within bounds
	if (RowNames.IsValidIndex(RowIndex))
	{
		// Iterate through all rows and look for one with the matching tag
		for (const FName& RowName : RowNames)
		{
			T* Row = DataTable->FindRow<T>(RowName, "");
			if (Row && Row->Tag == Tag)
			{
				// Check if this is the row at the desired index
				if (--RowIndex < 0)
				{
					return Row;
				}
			}
		}
	}

	UE_LOG(LogAria, Warning, TEXT("No matching row found"));
	return nullptr;
}

template <typename T>
T* UAriaAbilitySystemLibrary::GetAbilityDescriptionsRow(UDataTable* DataTable,
                                                        const FGameplayTag& AbilityTag,
                                                        int32 AbilityLevel)
{
	if (!DataTable)
	{
		UE_LOG(LogAria, Warning, TEXT("DataTable is nullptr"));
		return nullptr;
	}

	// Get all row names in the data table
	const TArray<FName> RowNames = DataTable->GetRowNames();

	// Iterate through all rows and look for one with the matching tag
	for (const FName& RowName : RowNames)
	{
		T* Row = DataTable->FindRow<T>(RowName, "");
		if (Row && Row->AbilityTag == AbilityTag &&
			Row->AbilityLevel == AbilityLevel)
		{
			return Row;
		}
	}

	UE_LOG(LogAria, Warning, TEXT("No matching row found"));
	return nullptr;
}
#pragma endregion
