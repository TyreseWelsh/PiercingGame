// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseState.h"
#include "PlayerOnWallState.generated.h"

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerOnWallState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	virtual void OnStateEnter(AActor* StateOwner) override;
	virtual void OnStateTick() override;
	virtual void OnStateExit() override;

	virtual void Move(const FInputActionValue& Value);
	virtual void Jump();

private:
	const float MAX_WALLSLIDE_SPEED = 300.f;
	bool bIsCollidingRight = false;
	bool bMovingTowardsWall = false;
	float LaunchVelX = 1000.f;
	float LaunchVelZ = 1800.f;
};
