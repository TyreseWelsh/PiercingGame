// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseState.h"
#include "PlayerPierceState.generated.h"

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerPierceState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	virtual void OnStateEnter(AActor* StateOwner) override;
	virtual void OnStateTick() override;
	virtual void OnStateExit() override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Jump() override;
	virtual void Attack() override;
	
private:
	void EnterPierce();
	UFUNCTION()
	void AimPierce();
	void EndPierce();
	UFUNCTION()
	void Pierce();
	void ResetPierceValues();
	void ReleasePierceButton();

	FTimerHandle PierceAimTimer;
	FTimerDelegate PierceAimDelegate;
	FTimerHandle PierceTravelTimer;
	FTimerDelegate PierceTravelDelegate;
};
