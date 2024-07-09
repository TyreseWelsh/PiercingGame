// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPierceState.h"

#include "LogicStateManagerComponent.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Pierceable.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerPierceState::OnStateEnter(AActor* StateOwner)
{
	Super::OnStateEnter(StateOwner);

	if(PlayerController)
	{
		PlayerController->GetReleasePierceDelegate()->AddUObject(this, &UPlayerPierceState::ReleasePierceButton);
	}

	if(!PierceAimDelegate.IsBound())
	{
		PierceAimDelegate.BindUFunction(this, "AimPierce");
	}
	if(!PierceTravelDelegate.IsBound())
	{
		PierceTravelDelegate.BindUFunction(this, "Pierce");
	}

	EnterPierce();
}

void UPlayerPierceState::OnStateTick()
{
	Super::OnStateTick();
}

void UPlayerPierceState::OnStateExit()
{
	Super::OnStateExit();

	ResetPierceValues();

	if(PlayerController)
	{
		PlayerController->GetReleasePierceDelegate()->RemoveAll(this);
	}
	
	//PierceAimDelegate.Unbind();
	//PierceTravelDelegate.Unbind();
}

void UPlayerPierceState::Move(const FInputActionValue& Value)
{
	
}

void UPlayerPierceState::Jump()
{
	
}

void UPlayerPierceState::Attack()
{

}

void UPlayerPierceState::EnterPierce()
{
	PlayerRef->bIsPierceButtonReleased = false;
	
	PlayerRef->GetWorldSettings()->SetTimeDilation(0.f);

	PlayerRef->GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);

	GetWorld()->GetTimerManager().SetTimer(PierceAimTimer, PierceAimDelegate, 0.0002f * GetWorld()->DeltaRealTimeSeconds, true);
}

void UPlayerPierceState::AimPierce()
{
	if(IsValid(PlayerRef))
	{
		FVector MouseLocation;
		FVector MouseDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		MouseLocation.Y = 0;
		
		FVector VectorFromTargetToMouse = FVector(PlayerRef->ClosestPierceTarget->GetActorLocation().X, 0.0f, PlayerRef->ClosestPierceTarget->GetActorLocation().Z) - FVector(MouseLocation.X, 0.f, MouseLocation.Z);
		
		VectorFromTargetToMouse.Normalize();
		PlayerRef->PierceDirection = VectorFromTargetToMouse * -1;

		PlayerRef->PierceDistanceMultiplier -= GetWorld()->DeltaRealTimeSeconds;
		if (PlayerRef->PierceDistanceMultiplier <= 0.35f)															// Distance multiplier was 0.4 but has been divided by 10 to account for delta times small values
		{
			PlayerRef->ClosestPierceTarget = nullptr;
			EndPierce();
			PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
			return;
		}

		// Pierce distance multiplied by PierceDistanceMultiplier so the distance covered is continuously reduced while the player holds the button, incentivising faster gameplay
		FVector PiercePower = (PlayerRef->PierceDirection * PlayerRef->PierceDistance) * PlayerRef->PierceDistanceMultiplier;
		PlayerRef->PierceEndPos = PlayerRef->ClosestPierceTarget->GetActorLocation() + PiercePower;
		PlayerRef->PierceStartPos = PlayerRef->ClosestPierceTarget->GetActorLocation() + (-1 * PiercePower / 2);


		if(!IsValid(PlayerRef->PierceAimHead))
		{
			FTransform PierceHeadTransform;
			PierceHeadTransform.SetLocation(PlayerRef->PierceEndPos);
			PierceHeadTransform.SetRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceEndPos, MouseLocation).Quaternion());
			
			PlayerRef->PierceAimHead = GetWorld()->SpawnActor<AActor>(PlayerRef->PierceHeadBP, PierceHeadTransform);
		}
		else
		{
			PlayerRef->PierceAimHead->SetActorLocation(PlayerRef->PierceEndPos);
			PlayerRef->PierceAimHead->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceEndPos, MouseLocation).Quaternion());
		}

		if(!IsValid(PlayerRef->PierceAimTail))
		{
			FTransform PierceTailTransform;
			PierceTailTransform.SetLocation(PlayerRef->PierceStartPos);
			PierceTailTransform.SetRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceStartPos, MouseLocation).Quaternion());

			PlayerRef->PierceAimTail = GetWorld()->SpawnActor<AActor>(PlayerRef->PierceTailBP, PierceTailTransform);
		}
		else
		{
			PlayerRef->PierceAimTail->SetActorLocation(PlayerRef->PierceStartPos);
			PlayerRef->PierceAimTail->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceStartPos, MouseLocation).Quaternion());
		}
		

		PlayerRef->PierceStartDelay += GetWorld()->DeltaRealTimeSeconds;
		if (PlayerRef->PierceStartDelay >= PlayerRef->MAX_PIERCE_START_DELAY && PlayerRef->bIsPierceButtonReleased)
		{
			EndPierce();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "Aim Pierce - Player Ref INVALID");

	}

}

void UPlayerPierceState::EndPierce()
{
	if(IsValid(PlayerRef))
	{
		PlayerRef->GetWorldSettings()->SetTimeDilation(1.f);

	
		if (PierceAimTimer.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(PierceAimTimer);
		}
		if(IsValid(PlayerRef->PierceAimHead))
		{
			PlayerRef->PierceAimHead->Destroy();
		}
		if(IsValid(PlayerRef->PierceAimTail))
		{
			PlayerRef->PierceAimTail->Destroy();
		}
	
	
		if (IsValid(PlayerRef->ClosestPierceTarget))
		{
			PlayerRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
			if (PlayerController)
			{
				PlayerRef->DisableInput(PlayerController);
				PlayerRef->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
				PlayerRef->SetActorLocation(PlayerRef->PierceStartPos, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);

				GetWorld()->GetTimerManager().SetTimer(PierceTravelTimer, PierceTravelDelegate, 0.01f, true);
			}
		}
	
		PlayerRef->PierceDistanceMultiplier = 1.f;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "End Pierce - Player Ref INVALID");
	}
}

void UPlayerPierceState::Pierce()
{
	if (PlayerRef->PierceTravelPercent <= 1.f)
	{
		FVector NewActorLocation = PlayerRef->PierceStartPos + (PlayerRef->PierceEndPos - PlayerRef->PierceStartPos) * PlayerRef->PierceTravelPercent;
		PlayerRef->SetActorLocation(NewActorLocation, true);
		PlayerRef->PierceTravelPercent += 0.05;

		if (PlayerRef->PierceTravelPercent >= 0.5f)
		{
			PlayerRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		}
	}
	else
	{
		PlayerRef->GetCharacterMovement()->Velocity = PlayerRef->PierceDirection * 1000.f;
		PlayerRef->EnableInput(PlayerController);
		GetWorld()->GetTimerManager().ClearTimer(PierceTravelTimer);
		PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("InAir");
	}
}

void UPlayerPierceState::ResetPierceValues()
{
	GetWorld()->GetTimerManager().ClearTimer(PierceAimTimer);
	GetWorld()->GetTimerManager().ClearTimer(PierceTravelTimer);
	
	PlayerRef->PierceDirection = FVector::Zero();
    PlayerRef->PierceStartPos = FVector::Zero();
    PlayerRef->PierceEndPos = FVector::Zero();
    PlayerRef->ClosestPierceTarget = nullptr;
    PlayerRef->PierceTravelPercent = 0.f;
    PlayerRef->JumpCurrentCount = 0;
}

void UPlayerPierceState::ReleasePierceButton()
{
	PlayerRef->bIsPierceButtonReleased = true;
}
