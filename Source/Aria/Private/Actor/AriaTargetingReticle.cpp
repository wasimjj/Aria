// Copyright Savoie Interactive Studios


#include "Actor/AriaTargetingReticle.h"

#include "Components/DecalComponent.h"

AAriaTargetingReticle::AAriaTargetingReticle()
{
 	PrimaryActorTick.bCanEverTick = true;

	TargetingReticleDecal = CreateDefaultSubobject<UDecalComponent>("TargetingReticleDecal");
	SetRootComponent(TargetingReticleDecal);
}

void AAriaTargetingReticle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAriaTargetingReticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
