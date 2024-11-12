// Copyright Savoie Interactive Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AriaTargetingReticle.generated.h"

UCLASS()
class ARIA_API AAriaTargetingReticle : public AActor
{
	GENERATED_BODY()
	
public:	
	AAriaTargetingReticle();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> TargetingReticleDecal;
	
protected:
	virtual void BeginPlay() override;
};
