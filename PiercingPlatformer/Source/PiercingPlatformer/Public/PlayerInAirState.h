// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseState.h"
#include "PlayerInAirState.generated.h"

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerInAirState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	virtual void OnStateEnter(AActor* StateOwner) override;
	virtual void OnStateTick() override;
	virtual void OnStateExit() override;

	virtual void Move(const FInputActionValue& Value);
	virtual void Jump();
	virtual void ReleaseJump();

	UFUNCTION()
	void StartHover();
	UFUNCTION()
	void Hover();

private:
	// Number of frames jump can be buffered for
	int JumpBufferFrameCounter = 5;

	bool bCanHover = false;
	const int MaxHoverFuel = 100;
	int CurrentHoverFuel = 100;

	FTimerHandle HoverTimer;
	FTimerDelegate HoverTimerDelegate;
};
