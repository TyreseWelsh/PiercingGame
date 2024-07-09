// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseState.h"
#include "PlayerMoveState.generated.h"

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerMoveState : public UPlayerBaseState
{
	GENERATED_BODY()

	virtual void OnStateTick() override;

};
