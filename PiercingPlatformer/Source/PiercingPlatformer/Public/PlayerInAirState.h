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
};
