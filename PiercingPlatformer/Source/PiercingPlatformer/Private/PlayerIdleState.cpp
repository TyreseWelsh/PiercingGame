// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerIdleState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerIdleState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	if(PlayerRef->bIsJumpBuffered)
	{
		PlayerRef->bIsJumpBuffered = false;
		Jump();
	}
}

void UPlayerIdleState::OnStateTick()
{
	Super::OnStateTick();
	
	if(PlayerRef->GetCharacterMovement()->Velocity.Z != 0)
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	}
}
