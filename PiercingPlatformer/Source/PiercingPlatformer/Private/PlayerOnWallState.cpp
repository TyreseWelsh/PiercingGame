// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerOnWallState.h"

#include "InputActionValue.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "LogicStateManagerComponent.h"

void UPlayerOnWallState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);
	//GEngine->AddOnScreenDebugMessage(int32(-1), 10.f, FColor::Green, "entererd on wall");

	if(IsValid(PlayerRef))
	{
		PlayerRef->JumpCurrentCount = 0;

		LaunchVelZ = PlayerRef->GetCharacterMovement()->JumpZVelocity;
	}
}

void UPlayerOnWallState::OnStateTick()
{
	Super::OnStateTick();

	if(IsValid(PlayerRef))
	{
		if(PlayerRef->GetCharacterMovement()->Velocity.Z < -MAX_WALLSLIDE_SPEED)
		{
			PlayerRef->GetCharacterMovement()->Velocity.Z = -MAX_WALLSLIDE_SPEED;
		}
		
		if(PlayerRef->GetCharacterMovement()->Velocity.X == 0 && !bMovingTowardsWall)
		{
			PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
		}

		if(PlayerRef->GetCharacterMovement()->Velocity.Z == 0)
		{
			PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Idle");
		}
	}

	// NOTE: If I want the player to need to hold movement towards the wall to keep them wall sliding
	//		 If commented, player does not need to continuously move towards wall to keep OnWallState
	//		 If uncommented, player will need to continuously move towards wall to keep OnWallState
	//bMovingTowardsWall = false;

	TArray<AActor*> LeftOverlappingWalls;
	PlayerRef->GetLeftWallCollider()->GetOverlappingActors(LeftOverlappingWalls);

	TArray<AActor*> RightOverlappingWalls;
	PlayerRef->GetRightWallCollider()->GetOverlappingActors(RightOverlappingWalls);
	
	if(LeftOverlappingWalls.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 10.f, FColor::Green, "left wall");

		CheckOverlappingWallCollision(PlayerRef->GetLeftWallCollider());
	}
	else if(RightOverlappingWalls.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 10.f, FColor::Green, "right wall");

		CheckOverlappingWallCollision(PlayerRef->GetRightWallCollider());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "overlap not ont");

		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	}
}

void UPlayerOnWallState::OnStateExit()
{
	Super::OnStateExit();
}

void UPlayerOnWallState::Move(const FInputActionValue& Value)
{
	if(Value.Get<float>() < 0 && !bIsCollidingRight)
	{
		bMovingTowardsWall = true;
		return;
	}
	if(Value.Get<float>() > 0 && bIsCollidingRight)
	{
		bMovingTowardsWall = true;
		return;
	}
	
	PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Move");
}

void UPlayerOnWallState::Jump()
{
	if(bIsCollidingRight)
	{
		PlayerRef->LaunchCharacter(FVector(-LaunchVelX,0,LaunchVelZ), true, true);
	}
	else
	{
		PlayerRef->LaunchCharacter(FVector(LaunchVelX, 0, LaunchVelZ), true, true);
	}

	PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
}

void UPlayerOnWallState::CheckOverlappingWallCollision(UBoxComponent* WallCollider)
{
	FHitResult hit;
	FVector ColliderLocation = WallCollider->GetComponentTransform().GetLocation();
	FVector ColliderTraceStartPos = FVector(ColliderLocation.X, 0, ColliderLocation.Z + WallCollider->GetScaledBoxExtent().Z);
	FVector ColliderTraceEndPos = FVector(ColliderLocation.X, 0, ColliderLocation.Z - WallCollider->GetScaledBoxExtent().Z);

	DrawDebugLine(GetWorld(), ColliderTraceStartPos, ColliderTraceEndPos, FColor::Green, false, 1, 0, 10.f);
		
	if(GetWorld()->LineTraceSingleByChannel(hit, ColliderTraceStartPos, ColliderTraceEndPos, ECollisionChannel::ECC_GameTraceChannel1))
	{
		if(hit.Location.X < PlayerRef->GetActorLocation().X)
		{
			bIsCollidingRight = false;
		}

		if(hit.Location.X > PlayerRef->GetActorLocation().X)
		{
			bIsCollidingRight = true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "no wall trace hit");
	}
}
