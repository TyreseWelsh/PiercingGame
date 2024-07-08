// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"


void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if(UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = Cast<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::PressMove);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressJump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJump);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressAttack);

		EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressPierce);
		//EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Completed, this, &APlayerCharacterController::ReleasePierceButton);
	}
}

FMoveSignature* APlayerCharacterController::GetMoveDelegate()
{
	return &MoveDelegate;
}

FJumpSignature* APlayerCharacterController::GetJumpDelegate()
{
	return &JumpDelegate;
}

FAttackSignature* APlayerCharacterController::GetAttackDelegate()
{
	return &AttackDelegate;
}

FPierceSignature* APlayerCharacterController::GetPierceDelegate()
{
	return &PierceDelegate;
}

void APlayerCharacterController::PressMove(const FInputActionValue& Value)
{
	if(MoveDelegate.IsBound())
	{
		MoveDelegate.Broadcast((Value));
	}
}

void APlayerCharacterController::PressJump()
{
	if(JumpDelegate.IsBound())
	{
		JumpDelegate.Broadcast();
	}
}

void APlayerCharacterController::PressAttack()
{
	if(AttackDelegate.IsBound())
	{
		AttackDelegate.Broadcast();
	}
}

void APlayerCharacterController::PressPierce()
{
	if(PierceDelegate.IsBound())
	{
		PierceDelegate.Broadcast();
	}
}
