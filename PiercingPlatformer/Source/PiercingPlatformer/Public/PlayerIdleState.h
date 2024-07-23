// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseState.h"
#include "PlayerIdleState.generated.h"

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerIdleState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	virtual void OnStateEnter(AActor* StateOwner) override;
	virtual void OnStateTick() override;
	// virtual void OnStateExit() override;
	//
	// virtual void Move(const FInputActionValue& Value) override;
	// virtual void Jump() override;
	// virtual void Attack() override;
	// virtual void StartPierce() override;
};
