// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerOnWallState.h"

#include "InputActionValue.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Components/BoxComponent.h"
#include "LogicStateManagerComponent.h"

void UPlayerOnWallState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	if(IsValid(PlayerRef))
	{
		PlayerRef->JumpCurrentCount = 0;
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
	}

	bMovingTowardsWall = false;

	TArray<AActor*> OverlappingWalls;
	PlayerRef->GetRightWallCollider()->GetOverlappingActors(OverlappingWalls);
	if(OverlappingWalls.Num() > 0)
	{
		FHitResult hit;
		FVector ColliderLocation = PlayerRef->GetRightWallCollider()->GetComponentTransform().GetLocation();
		FVector ColliderTraceStartPos = FVector(ColliderLocation.X, 0, ColliderLocation.Z + PlayerRef->GetRightWallCollider()->GetUnscaledBoxExtent().Z);
		FVector ColliderTraceEndPos = FVector(ColliderLocation.X, 0, ColliderLocation.Z - PlayerRef->GetRightWallCollider()->GetUnscaledBoxExtent().Z);

		DrawDebugLine(GetWorld(), ColliderTraceStartPos, ColliderTraceEndPos, FColor::Green, false, 1, 0, 10.f);
		
		if(GetWorld()->LineTraceSingleByChannel(hit, ColliderTraceStartPos, ColliderTraceEndPos, ECollisionChannel::ECC_GameTraceChannel1))
		{
			if(hit.Location.X < PlayerRef->GetActorLocation().X)
			{
				GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "colliding left");

				bIsCollidingRight = false;
			}
			if(hit.Location.X > PlayerRef->GetActorLocation().X)
			{
				GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "colliding right");

				bIsCollidingRight = true;
			}
		}
	}
	else
	{
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
