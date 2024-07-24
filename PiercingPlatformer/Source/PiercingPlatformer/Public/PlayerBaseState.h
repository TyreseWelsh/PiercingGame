// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicStateBase.h"
#include "PlayerBaseState.generated.h"


class APlayerCharacter;
class APlayerCharacterController;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API UPlayerBaseState : public ULogicStateBase
{
	GENERATED_BODY()

public:
	APlayerCharacter* PlayerRef = nullptr;
	APlayerCharacterController* PlayerController = nullptr;

	virtual void OnStateEnter(AActor* StateOwner) override;
	virtual void OnStateTick() override;
	virtual void OnStateExit() override;

	virtual void Move(const FInputActionValue& Value);
	virtual void Jump();
	virtual void ReleaseJump();
	virtual void Attack();
	virtual void PressPierce();
	virtual void PressThrust();

protected:
	bool bWillCutJump = false;
};