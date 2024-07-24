// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInAirState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"

void UPlayerInAirState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);
}

void UPlayerInAirState::OnStateTick()
{
	Super::OnStateTick();
	
	if(JumpBufferFrameCounter > 0 && PlayerRef->bIsJumpBuffered)
	{
		JumpBufferFrameCounter -= 1;
	}
	else
	{
		PlayerRef->bIsJumpBuffered = false;
	}
	
	if(PlayerRef->GetCharacterMovement()->Velocity.Z == 0 && PlayerRef->GetCharacterMovement()->IsMovingOnGround())
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Idle");
	}
}

void UPlayerInAirState::OnStateExit()
{
	Super::OnStateExit();
	
	//PlayerRef->GetWallCollider()->OnComponentBeginOverlap.RemoveAll(this);
}

void UPlayerInAirState::Move(const FInputActionValue& Value)
{
	PlayerRef->AddMovementInput(FVector(1.f, 0.f, 0.f), Value.Get<float>());
	
	TArray<AActor*> LeftOverlappingWalls;
	PlayerRef->GetLeftWallCollider()->GetOverlappingActors(LeftOverlappingWalls);
	TArray<AActor*> RightOverlappingWalls;
	PlayerRef->GetRightWallCollider()->GetOverlappingActors(RightOverlappingWalls);

	if(RightOverlappingWalls.Num() > 0 )
	{
		PlayerRef->bIsJumpBuffered = false;
		PlayerRef->bIsWallOnRight = false;
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("OnWall");
	}
	else if(LeftOverlappingWalls.Num() > 0)
	{
		PlayerRef->bIsJumpBuffered = false;
		PlayerRef->bIsWallOnRight = true;
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("OnWall");
	}
}

void UPlayerInAirState::Jump()
{
	if(PlayerRef->JumpCurrentCount >= PlayerRef->JumpMaxCount)
	{
		PlayerRef->bIsJumpBuffered = true;
	}
	
	Super::Jump();
}

void UPlayerInAirState::ReleaseJump()
{
	if(PlayerRef->GetVelocity().Z > 0 && PlayerRef->GetVelocity().Z && bWillCutJump)
	{
		//PlayerRef->GetCharacterMovement()->Velocity.Z = PlayerRef->GetCharacterMovement()->JumpZVelocity / 2.25;
		PlayerRef->GetCharacterMovement()->Velocity.Z = 0;
	}
	
	Super::ReleaseJump();
}
