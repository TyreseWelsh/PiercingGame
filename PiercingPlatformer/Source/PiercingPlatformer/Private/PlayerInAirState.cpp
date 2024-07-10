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

	if(IsValid(PlayerRef))
	{
		PlayerRef->GetLeftWallCollider()->OnComponentBeginOverlap.AddDynamic(this, &UPlayerInAirState::OnBeginOverlap);
		PlayerRef->GetRightWallCollider()->OnComponentBeginOverlap.AddDynamic(this, &UPlayerInAirState::OnBeginOverlap);

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

void UPlayerInAirState::OnStateExit()
{
	Super::OnStateExit();

	PlayerRef->GetLeftWallCollider()->OnComponentBeginOverlap.RemoveAll(this);
	PlayerRef->GetRightWallCollider()->OnComponentBeginOverlap.RemoveAll(this);
}

void UPlayerInAirState::Move(const FInputActionValue& Value)
{
	PlayerRef->AddMovementInput(FVector(1.f, 0.f, 0.f), Value.Get<float>());
	
	TArray<AActor*> RightOverlappingWalls;
	PlayerRef->GetRightWallCollider()->GetOverlappingActors(RightOverlappingWalls);

	if(RightOverlappingWalls.Num() > 0)
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("OnWall");
	}

}