// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerMoveState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);
}

void UPlayerMoveState::OnStateTick()
{
	Super::OnStateTick();

	if(PlayerRef->GetCharacterMovement()->Velocity.X == 0)
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Idle");
	}
	if(PlayerRef->GetCharacterMovement()->Velocity.Z != 0)
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	}
}
