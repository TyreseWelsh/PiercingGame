// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseState.h"
#include "PlayerThrustState.generated.h"

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerThrustState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	virtual void OnStateEnter(AActor* StateOwner) override;
	virtual void OnStateTick() override;
	virtual void OnStateExit() override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Jump() override;
	virtual void ReleaseJump() override;
	virtual void Attack() override;
	virtual void PressPierce() override;
	virtual void PressThrust() override;

private:
	void StartThrust();
	UFUNCTION()
	void Thrust();

	void ResetThrustValues();

	float ThrustPower = 300;
	FVector ThrustDirection = FVector::Zero();
	FVector ThrustVector = FVector::Zero();
	FVector ThrustStartPos = FVector::Zero();
	FVector ThrustEndPos = FVector::Zero();
	
	float ThrustTravelPercent = 0.f;
	FTimerHandle ThrustTravelTimer;
	FTimerDelegate ThrustTravelDelegate;

};
