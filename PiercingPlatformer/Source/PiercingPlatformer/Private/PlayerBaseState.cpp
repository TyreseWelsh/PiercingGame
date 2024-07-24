// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseState.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Kismet/GameplayStatics.h"

// Includes for PlayerRef
#include "InputActionValue.h"
#include "LogicStateManagerComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Pierceable.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// For copying
//GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "Text!");


void UPlayerBaseState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	if(!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(StateOwner);
	}

	if(!PlayerController)
	{
		PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(this, 0));
	}

	// Binding action delegates
	if(PlayerController)
	{
		PlayerController->GetMoveDelegate()->AddUObject(this, &UPlayerBaseState::Move);
		PlayerController->GetJumpDelegate()->AddUObject(this, &UPlayerBaseState::Jump);
		PlayerController->GetReleaseJumpDelegate()->AddUObject(this, &UPlayerBaseState::ReleaseJump);
		PlayerController->GetAttackDelegate()->AddUObject(this, &UPlayerBaseState::Attack);
		PlayerController->GetPierceDelegate()->AddUObject(this, &UPlayerBaseState::PressPierce);
		PlayerController->GetThrustDelegate()->AddUObject(this, &UPlayerBaseState::PressThrust);
	}
	
	bWillCutJump = false;
}

void UPlayerBaseState::OnStateTick()
{
	Super::OnStateTick();

	FVector MouseLocation = FVector::Zero();
	FVector MouseDirection = FVector::Zero();

	PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	if (MouseLocation.X > PlayerRef->GetActorLocation().X)
	{
		// Mouse on right side of player
		PlayerRef->GetSprite()->SetRelativeScale3D(FVector(1, 1, 1));

		if(PlayerRef->CurrentMovementDirection > 0)
		{
			PlayerRef->bIsRunningForward = true;
		}
		else
		{
			PlayerRef->bIsRunningForward = false;
		}
	}
	else if(MouseLocation.X < PlayerRef->GetActorLocation().X)
	{
		// Mouse on left side of player
		PlayerRef->GetSprite()->SetRelativeScale3D(FVector(-1, 1, 1));

		if(PlayerRef->CurrentMovementDirection < 0)
		{
			PlayerRef->bIsRunningForward = true;
		}
		else
		{
			PlayerRef->bIsRunningForward = false;
		}
	}
}

void UPlayerBaseState::OnStateExit()
{
	Super::OnStateExit();

	if(PlayerController)
	{
		PlayerController->GetMoveDelegate()->RemoveAll(this);
		PlayerController->GetJumpDelegate()->RemoveAll(this);
		PlayerController->GetAttackDelegate()->RemoveAll(this);
		PlayerController->GetPierceDelegate()->RemoveAll(this);
		PlayerController->GetThrustDelegate()->RemoveAll(this);
	}
}

void UPlayerBaseState::Move(const FInputActionValue& Value)
{
	PlayerRef->CurrentMovementDirection = Value.Get<float>();
	PlayerRef->AddMovementInput(FVector(1.f, 0.f, 0.f), Value.Get<float>());
	PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Move");
}

void UPlayerBaseState::Jump()
{
	if(PlayerRef->JumpCurrentCount < PlayerRef->JumpMaxCount)
	{
		bWillCutJump = true;
		PlayerRef->Jump();
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	}
}

void UPlayerBaseState::ReleaseJump()
{
	PlayerRef->StopJumping();
}

void UPlayerBaseState::Attack()
{
	PlayerRef->GetAnimInstance()->PlayAnimationOverride(PlayerRef->BasicAttackAnim, "DefaultSlot", 1.f, 0.f);
}

void UPlayerBaseState::PressPierce()
{
	TSet<AActor*> PierceableTargets;
	PlayerRef->GetPierceRadiusComponent()->GetOverlappingActors(PierceableTargets);

	float DistanceToClosestTarget = 99999.f;

	for (AActor* target : PierceableTargets)
	{
		if (IPierceable* PiercableInterface = Cast<IPierceable>(target))
		{
			if (PiercableInterface != nullptr)
			{
				float distanceToTarget = FVector::Distance(target->GetActorLocation(), PlayerRef->GetActorLocation());

				if (distanceToTarget < DistanceToClosestTarget)
				{
					DistanceToClosestTarget = distanceToTarget;
					PlayerRef->ClosestPierceTarget = target;
				}
			}
		}
	}

	if (IsValid(PlayerRef->ClosestPierceTarget))
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Pierce");
	}
}

void UPlayerBaseState::PressThrust()
{
	if(IsValid(PlayerRef))
	{
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Thrust");
	}
}
