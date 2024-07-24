// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerThrustState.h"

#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "LogicStateManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerThrustState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);
	
	if(!ThrustTravelDelegate.IsBound())
	{
		ThrustTravelDelegate.BindUFunction(this, "Thrust");
	}

	StartThrust();
}

void UPlayerThrustState::OnStateTick()
{
	Super::OnStateTick();
}

void UPlayerThrustState::OnStateExit()
{
	Super::OnStateExit();
	
	if(IsValid(PlayerRef))
	{
		PlayerRef->JumpCurrentCount = 0;
	}

	ResetThrustValues();
}

void UPlayerThrustState::Move(const FInputActionValue& Value)
{
	
}

void UPlayerThrustState::Jump()
{
	
}

void UPlayerThrustState::ReleaseJump()
{
	
}

void UPlayerThrustState::Attack()
{
	
}

void UPlayerThrustState::PressPierce()
{
	
}

void UPlayerThrustState::PressThrust()
{
	
}

void UPlayerThrustState::StartThrust()
{
	if(IsValid(PlayerRef))
	{
		FVector MouseLocation = FVector::Zero();
		FVector MouseDirection = FVector::Zero();

		PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

		ThrustDirection = FVector(MouseLocation.X, 0.f, MouseLocation.Z) - FVector(PlayerRef->GetActorLocation().X, 0.0f, PlayerRef->GetActorLocation().Z);

		ThrustDirection.Normalize();
		ThrustVector = ThrustDirection * ThrustPower;

		ThrustStartPos = PlayerRef->GetActorLocation();
		ThrustEndPos = PlayerRef->GetActorLocation() + ThrustVector;
	}
	
	GetWorld()->GetTimerManager().SetTimer(ThrustTravelTimer, ThrustTravelDelegate, 0.2f * GetWorld()->DeltaRealTimeSeconds, true);
}

void UPlayerThrustState::Thrust()
{
	if(ThrustTravelPercent <= 1.f)
	{
		FVector NewActorLocation = ThrustStartPos + (ThrustVector * ThrustTravelPercent);
		PlayerRef->SetActorLocation(NewActorLocation, true);
		ThrustTravelPercent += 0.025f;
	}
	else
	{
		PlayerRef->GetCharacterMovement()->Velocity = ThrustDirection * 1200.f;
		GetWorld()->GetTimerManager().ClearTimer(ThrustTravelTimer);
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	}
}

void UPlayerThrustState::ResetThrustValues()
{
	ThrustDirection = FVector::Zero();
	ThrustVector = FVector::Zero();
	ThrustStartPos = FVector::Zero();
	ThrustEndPos = FVector::Zero();
	
	ThrustTravelPercent = 0.f;
}
