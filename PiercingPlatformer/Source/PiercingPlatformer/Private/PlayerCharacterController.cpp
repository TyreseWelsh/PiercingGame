// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"


void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = Cast<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Red, "NOT EVEN adding mapping context");
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::PressMove);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::ReleaseJump);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressAttack);

		EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressPierce);
		EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Completed, this, &APlayerCharacterController::ReleasePierce);

		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Started, this, &APlayerCharacterController::PressThrust);
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

FReleaseJumpSignature* APlayerCharacterController::GetReleaseJumpDelegate()
{
	return &ReleaseJumpDelegate;
}

FAttackSignature* APlayerCharacterController::GetAttackDelegate()
{
	return &AttackDelegate;
}

FPierceSignature* APlayerCharacterController::GetPierceDelegate()
{
	return &PierceDelegate;
}

FReleasePierceSignature* APlayerCharacterController::GetReleasePierceDelegate()
{
	return &ReleasePierceDelegate;
}

FThrustSignature* APlayerCharacterController::GetThrustDelegate()
{
	return &ThrustDelegate;
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

void APlayerCharacterController::ReleaseJump()
{
	if(ReleaseJumpDelegate.IsBound())
	{
		ReleaseJumpDelegate.Broadcast();
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

void APlayerCharacterController::ReleasePierce()
{
	if(ReleasePierceDelegate.IsBound())
	{
		ReleasePierceDelegate.Broadcast();
	}
}

void APlayerCharacterController::PressThrust()
{
	if(ThrustDelegate.IsBound())
	{
		ThrustDelegate.Broadcast();
	}
}
