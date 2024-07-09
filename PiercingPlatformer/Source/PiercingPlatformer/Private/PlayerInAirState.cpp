// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInAirState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerInAirState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	if(IsValid(PlayerRef))
	{
		if(PlayerRef->JumpCurrentCount == 1)
		{
			//GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "Double jump!");
		}
	}
}

void UPlayerInAirState::OnStateTick()
{
	Super::OnStateTick();

	if(PlayerRef->GetCharacterMovement()->Velocity.Z == 0)
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Idle");
	}
}
