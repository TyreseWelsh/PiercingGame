// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerMoveState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	//CoyoteTimer = 0.f;
}

void UPlayerMoveState::OnStateTick()
{
	Super::OnStateTick();

	if(PlayerRef->GetCharacterMovement()->Velocity.X == 0)
	{
		//PlayerRef->GetCharacterMovement()->GravityScale = 4.5;

		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Idle");
	}
	// if(PlayerRef->GetCharacterMovement()->Velocity.Z != 0)
	// {
	// 	PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	// }

	CheckCoyoteTime();

}

void UPlayerMoveState::CheckCoyoteTime()
{
	if(PlayerRef->GetCharacterMovement()->Velocity.Z >= 0)
	{
		CoyoteTimer = 0.f;
		PreviousZPos = PlayerRef->GetActorLocation().Z;

		if(PlayerRef->GetCharacterMovement()->Velocity.Z > 0)
		{
			PlayerRef->GetCharacterMovement()->GravityScale = 4.5;

			//PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
		}
	}
	else if(PlayerRef->GetCharacterMovement()->Velocity.Z < 0)
	{
		CoyoteTimer += GetWorld()->DeltaRealTimeSeconds;
		if(CoyoteTimer >= MAX_COYOTE_TIME)
		{
			CoyoteTimer = 0.f;
			//PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			//PlayerRef->GetCharacterMovement()->GravityScale = 4.5f;

			PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
			//GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "Coyote Time - Ended");

		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "Coyote Time!");

			PlayerRef->GetCharacterMovement()->Velocity.Z = 0;
			//PlayerRef->GetCharacterMovement()->GravityScale = 0;
			PlayerRef->SetActorLocation(FVector(PlayerRef->GetActorLocation().X, PlayerRef->GetActorLocation().Y, PreviousZPos));
			PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}
