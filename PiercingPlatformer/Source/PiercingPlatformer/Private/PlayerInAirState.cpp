// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInAirState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"
#include "PlayerCharacterController.h"

void UPlayerInAirState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	if(PlayerController)
	{
		PlayerController->GetHoverDelegate()->AddUObject(this, &UPlayerInAirState::StartHover);
	}

	// NOTE: Re-enable if WIP hovering should be re-enabled
	// if(!HoverTimerDelegate.IsBound())
	// {
	// 	HoverTimerDelegate.BindUFunction(this, "Hover");
	// }

	bWillCutJump = true;
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
	
	GetWorld()->GetTimerManager().ClearTimer(HoverTimer);
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
		bCanHover = true;
	}
	
	Super::Jump();
}

void UPlayerInAirState::ReleaseJump()
{
	if(PlayerRef->GetVelocity().Z > 0 && bWillCutJump)
	{
		PlayerRef->GetCharacterMovement()->Velocity.Z = 0;
	}
	
	Super::ReleaseJump();
}

void UPlayerInAirState::StartHover()
{
	if(bCanHover && CurrentHoverFuel > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(HoverTimer, HoverTimerDelegate, 0.02f, true);
	}
}

void UPlayerInAirState::Hover()
{
	GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "Hovering!");
}
