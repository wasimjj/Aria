// Copyright Savoie Interactive Studios


#include "Actor/AriaPointCollection.h"

#include "AbilitySystem/AriaAbilitySystemLibrary.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAriaPointCollection::AAriaPointCollection()
{
    PrimaryActorTick.bCanEverTick = false;

	// Load the TargetIcon sprite assets
	static ConstructorHelpers::FObjectFinder<UTexture2D> TargetSpawnIconAsset(TEXT("/Engine/EditorMaterials/TargetIconSpawn"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TargetIconAsset(TEXT("/Engine/EditorMaterials/TargetIcon"));

	if (!TargetSpawnIconAsset.Succeeded() || !TargetIconAsset.Succeeded()) return;
	
    // Initialize Pt_00 and its Billboard
    Pt_00 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_00"));
    PointsCollection.Add(Pt_00);
    SetRootComponent(Pt_00);

    Billboard_00 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_00"));
    Billboard_00->SetupAttachment(Pt_00);
	Billboard_00->SetSprite(TargetSpawnIconAsset.Object);
	
    // Initialize Pt_01 and its Billboard
    Pt_01 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_01"));
    PointsCollection.Add(Pt_01);
    Pt_01->SetupAttachment(GetRootComponent());

    Billboard_01 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_01"));
    Billboard_01->SetupAttachment(Pt_01);
	Billboard_01->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_02 and its Billboard
    Pt_02 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_02"));
    PointsCollection.Add(Pt_02);
    Pt_02->SetupAttachment(GetRootComponent());

    Billboard_02 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_02"));
    Billboard_02->SetupAttachment(Pt_02);
	Billboard_02->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_03 and its Billboard
    Pt_03 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_03"));
    PointsCollection.Add(Pt_03);
    Pt_03->SetupAttachment(GetRootComponent());

    Billboard_03 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_03"));
    Billboard_03->SetupAttachment(Pt_03);
	Billboard_03->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_04 and its Billboard
    Pt_04 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_04"));
    PointsCollection.Add(Pt_04);
    Pt_04->SetupAttachment(GetRootComponent());

    Billboard_04 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_04"));
    Billboard_04->SetupAttachment(Pt_04);
	Billboard_04->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_05 and its Billboard
    Pt_05 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_05"));
    PointsCollection.Add(Pt_05);
    Pt_05->SetupAttachment(GetRootComponent());

    Billboard_05 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_05"));
    Billboard_05->SetupAttachment(Pt_05);
	Billboard_05->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_06 and its Billboard
    Pt_06 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_06"));
    PointsCollection.Add(Pt_06);
    Pt_06->SetupAttachment(GetRootComponent());

    Billboard_06 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_06"));
    Billboard_06->SetupAttachment(Pt_06);
	Billboard_06->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_07 and its Billboard
    Pt_07 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_07"));
    PointsCollection.Add(Pt_07);
    Pt_07->SetupAttachment(GetRootComponent());

    Billboard_07 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_07"));
    Billboard_07->SetupAttachment(Pt_07);
	Billboard_07->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_08 and its Billboard
    Pt_08 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_08"));
    PointsCollection.Add(Pt_08);
    Pt_08->SetupAttachment(GetRootComponent());

    Billboard_08 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_08"));
    Billboard_08->SetupAttachment(Pt_08);
	Billboard_08->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_09 and its Billboard
    Pt_09 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_09"));
    PointsCollection.Add(Pt_09);
    Pt_09->SetupAttachment(GetRootComponent());

    Billboard_09 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_09"));
    Billboard_09->SetupAttachment(Pt_09);
	Billboard_09->SetSprite(TargetIconAsset.Object);
	
    // Initialize Pt_10 and its Billboard
    Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_10"));
    PointsCollection.Add(Pt_10);
    Pt_10->SetupAttachment(GetRootComponent());

    Billboard_10 = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard_10"));
    Billboard_10->SetupAttachment(Pt_10);
	Billboard_10->SetSprite(TargetIconAsset.Object);
}

TArray<USceneComponent*> AAriaPointCollection::GetGroundPoints(const FVector& GroundOriginLoc,
                                                               const float GroundRadius,
                                                               const int32 NumPtsToInclude,
                                                               const float OverrideOriginRotation,
                                                               const bool bIgnoreRadius,
                                                               const bool bDrawDebugSpheres)
{
	// Make sure that the Scene Component Points is greater than or equal to the Num Points requested 
	checkf(PointsCollection.Num() >= NumPtsToInclude, TEXT("Attempted to Access PointsCollection Out of Bounds"));

	// Makes a copy of the original Points; so if anything changes in the array, the original will be safe
	TArray<USceneComponent*> PointsCollectionCopy;
	for (USceneComponent* Pt : PointsCollection)
	{
		if (PointsCollectionCopy.Num() >= NumPtsToInclude) return PointsCollectionCopy;
		
		// Get the Location of all the Points in the Points Collection
		// Also Override the Rotation of the Points by the OverrideOriginRotation amount where applicable
		if (Pt != Pt_00)
		{
			// The FVector from the Center (Pt_0) to the new Point (Pt)
			FVector ToPoint = Pt->GetComponentLocation() - Pt_00->GetComponentLocation();
			// OverrideOriginRotation when set will Rotate all Points around
			//  the Origin Point by that Rotation amount (between 0 and 360 for full rotation)
			ToPoint = ToPoint.RotateAngleAxis(OverrideOriginRotation, FVector::UpVector);

			Pt->SetWorldLocation(Pt_00->GetComponentLocation() + ToPoint);
		}

		// Check if the point is within the GroundRadius
		float DistanceToOrigin = FVector::Dist(Pt->GetComponentLocation(), GroundOriginLoc);
		if (!bIgnoreRadius && DistanceToOrigin > GroundRadius)
		{
			continue;
		}
		
		// Perform a Line Trace straight down to the floor to get the Ground location
		// RaisedLocation is the Trace Start location which will start higher than the Ground
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		// LoweredLocation is the Trace End location which will be at a position lower than the Ground
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		// Get the collection of Actors around the Line Trace; these will be used to tell the Line Trace to Ignore any of those Actors 
		TArray<AActor*> ActorsToIgnore;
		UAriaAbilitySystemLibrary::GetLiveActorsWithinRadius(this, ActorsToIgnore, TArray<AActor*>(), GroundRadius, GetActorLocation());

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		// Ignoring all of the Live Actors within the Trace Radius
		QueryParams.AddIgnoredActors(ActorsToIgnore);
		// Using LineTraceByProfile, so that this will only Trace against Objects
		//  that have a specific Collision profile
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		// Note: The Normal is the Vector from the Ground straight out perpendicular
		//  (Known as 'Orthogonal' when we use 3D Vectors to that face that was hit )

		// Debugging
		if (bDrawDebugSpheres)
		{
			if (const UWorld* World = this->GetWorld())
			{
				DrawDebugSphere(World, Pt->GetComponentLocation(), 20, 12, FColor::White, false, 1.5f);
			}
		}
		
		PointsCollectionCopy.Add(Pt);
	}

	return PointsCollectionCopy;
}

void AAriaPointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
