// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseState.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Kismet/GameplayStatics.h"

#include "InputActionValue.h"

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
		PlayerController->GetAttackDelegate()->AddUObject(this, &UPlayerBaseState::Attack);
		PlayerController->GetPierceDelegate()->AddUObject(this, &UPlayerBaseState::Pierce);
	}
}

void UPlayerBaseState::OnStateTick()
{
	Super::OnStateTick();
}

void UPlayerBaseState::OnStateExit()
{
	Super::OnStateExit();
}
