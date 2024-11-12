// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AriaPointCollection.generated.h"

UCLASS()
class ARIA_API AAriaPointCollection : public AActor
{
	GENERATED_BODY()

public:
	AAriaPointCollection();

	// Returns the preset # of Points on the Ground
	//  Note: Making this Blueprint Pure, as everytime this gets called, we want it to be executed afresh
	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundOriginLoc,
	                                         const float GroundRadius = 500.f,
	                                         const int32 NumPtsToInclude = 1,
	                                         const float OverrideOriginRotation = 0.f,
	                                         const bool bIgnoreRadius = false,
	                                         const bool bDrawDebugSpheres = false);

protected:
	virtual void BeginPlay() override;

	// Scene Components are more useful than FVectors because these can be placed
	//  where we need them in relation to the Root Component
	// Scene Components also are more beneficial because they have not just Location
	//  but also Rotation
	//	 Example: Using the Scene Component Rotation, we can even angle our Particle Systems
	//			  that we spawn based on, for example, the Normal of the ground

	// A collection of Scene Components; one for each Point location
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<USceneComponent*> PointsCollection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_00;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_00;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_01;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_01;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_02;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_02;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_03;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_03;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_04;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_04;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_05;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_05;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_06;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_06;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_07;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_07;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_08;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_08;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_09;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_09;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_10;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> Billboard_10;
};
