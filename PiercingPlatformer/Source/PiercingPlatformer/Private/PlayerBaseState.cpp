// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseState.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Kismet/GameplayStatics.h"

// Includes for PlayerRef
#include "InputActionValue.h"
#include "LogicStateManagerComponent.h"
#include "PaperZDAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Pierceable.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
	}
}

void UPlayerBaseState::OnStateTick()
{
	Super::OnStateTick();
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
	}
}

void UPlayerBaseState::Move(const FInputActionValue& Value)
{
	PlayerRef->AddMovementInput(FVector(1.f, 0.f, 0.f), Value.Get<float>());
	PlayerRef->GetLogicStateManagerComponent()->SwitchStateByKey("Move");
}

void UPlayerBaseState::Jump()
{
	if(PlayerRef->JumpCurrentCount < PlayerRef->JumpMaxCount)
	{
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

// void UPlayerBaseState::StartPierce()
// {
// 	TSet<AActor*> PierceableTargets;
// 	PlayerRef->GetPierceRadiusComponent()->GetOverlappingActors(PierceableTargets);
//
// 	float DistanceToClosestTarget = 9999.f;
//
// 	for (AActor* target : PierceableTargets)
// 	{
// 		if (IPierceable* PiercableInterface = Cast<IPierceable>(target))
// 		{
// 			if (PiercableInterface != nullptr)
// 			{
// 				float distanceToTarget = FVector::Distance(target->GetActorLocation(), PlayerRef->GetActorLocation());
//
// 				if (distanceToTarget < DistanceToClosestTarget)
// 				{
// 					DistanceToClosestTarget = distanceToTarget;
// 					PlayerRef->ClosestPierceTarget = target;
// 				}
// 			}
// 		}
// 	}
//
// 	if (PlayerRef->ClosestPierceTarget == nullptr)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("NO TARGETS"));
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "Start pierce");
//
//
// 		PlayerRef->bIsPierceButtonReleased = false;
// 		
// 		PlayerRef->GetWorldSettings()->SetTimeDilation(0.f);
//
// 		PlayerRef->GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
//
// 		GetWorld()->GetTimerManager().SetTimer(PlayerRef->PierceAimTimer, PlayerRef->PierceAimDelegate, 0.0002f * GetWorld()->DeltaRealTimeSeconds, true);
// 	}
// }
//
// void UPlayerBaseState::AimPierce()
// {
// 	FVector MouseLocation;
// 	FVector MouseDirection;
// 	PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
// 	MouseLocation.Y = 0;
// 	
// 	FVector VectorFromTargetToMouse = FVector(PlayerRef->ClosestPierceTarget->GetActorLocation().X, 0.0f, PlayerRef->ClosestPierceTarget->GetActorLocation().Z) - FVector(MouseLocation.X, 0.f, MouseLocation.Z);
// 	
// 	VectorFromTargetToMouse.Normalize();
// 	PlayerRef->PierceDirection = VectorFromTargetToMouse * -1;
//
// 	PlayerRef->PierceDistanceMultiplier -= GetWorld()->DeltaRealTimeSeconds;
// 	if (PlayerRef->PierceDistanceMultiplier <= 0.35f)															// Distance multiplier was 0.4 but has been divided by 10 to account for delta times small values
// 	{
// 		// NOTE: Ideally exit the pierce state which will cancel the pierce all together easily
// 		PlayerRef->ClosestPierceTarget = nullptr;
// 		EndPierce();
// 		return;
// 	}
//
// 	// Pierce distance multiplied by PierceDistanceMultiplier so the distance covered is continuously reduced while the player holds the button, incentivising faster gameplay
// 	FVector PiercePower = (PlayerRef->PierceDirection * PlayerRef->PierceDistance) * PlayerRef->PierceDistanceMultiplier;
// 	PlayerRef->PierceEndPos = PlayerRef->ClosestPierceTarget->GetActorLocation() + PiercePower;
// 	PlayerRef->PierceStartPos = PlayerRef->ClosestPierceTarget->GetActorLocation() + (-1 * PiercePower / 2);
//
//
// 	if(!IsValid(PlayerRef->PierceAimHead))
// 	{
// 		FTransform PierceHeadTransform;
// 		PierceHeadTransform.SetLocation(PlayerRef->PierceEndPos);
// 		PierceHeadTransform.SetRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceEndPos, MouseLocation).Quaternion());
// 		
// 		PlayerRef->PierceAimHead = GetWorld()->SpawnActor<AActor>(PlayerRef->PierceHeadBP, PierceHeadTransform);
// 	}
// 	else
// 	{
// 		PlayerRef->PierceAimHead->SetActorLocation(PlayerRef->PierceEndPos);
// 		PlayerRef->PierceAimHead->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceEndPos, MouseLocation).Quaternion());
// 	}
//
// 	if(!IsValid(PlayerRef->PierceAimTail))
// 	{
// 		FTransform PierceTailTransform;
// 		PierceTailTransform.SetLocation(PlayerRef->PierceStartPos);
// 		PierceTailTransform.SetRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceStartPos, MouseLocation).Quaternion());
//
// 		PlayerRef->PierceAimTail = GetWorld()->SpawnActor<AActor>(PlayerRef->PierceTailBP, PierceTailTransform);
// 	}
// 	else
// 	{
// 		PlayerRef->PierceAimTail->SetActorLocation(PlayerRef->PierceStartPos);
// 		PlayerRef->PierceAimTail->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerRef->PierceStartPos, MouseLocation).Quaternion());
// 	}
// 	
//
// 	PlayerRef->PierceStartDelay += GetWorld()->DeltaRealTimeSeconds;
// 	if (PlayerRef->PierceStartDelay >= PlayerRef->MAX_PIERCE_START_DELAY && PlayerRef->bIsPierceButtonReleased)
// 	{
// 		EndPierce();
// 	}
// }
//
// void UPlayerBaseState::EndPierce()
// {
// 	PlayerRef->GetWorldSettings()->SetTimeDilation(1.f);
//
// 	if (IsValid(PlayerRef->ClosestPierceTarget))
// 	{
// 		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "End pierce");
//
// 		PlayerRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
// 		if (PlayerController)
// 		{
// 			PlayerRef->DisableInput(PlayerController);
// 			PlayerRef->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
// 			PlayerRef->SetActorLocation(PlayerRef->PierceStartPos, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
//
// 			GetWorld()->GetTimerManager().SetTimer(PlayerRef->PierceTravelTimer, PlayerRef->PierceTravelDelegate, 0.01f, true);
// 		}
// 	}
//
// 	if (PlayerRef->PierceAimTimer.IsValid())
// 	{
// 		GetWorld()->GetTimerManager().ClearTimer(PlayerRef->PierceAimTimer);
// 	}
// 	if(IsValid(PlayerRef->PierceAimHead))
// 	{
// 		PlayerRef->PierceAimHead->Destroy();
// 	}
// 	if(IsValid(PlayerRef->PierceAimTail))
// 	{
// 		PlayerRef->PierceAimTail->Destroy();
// 	}
// 	PlayerRef->PierceDistanceMultiplier = 1.f;
// }
//
// void UPlayerBaseState::Pierce()
// {
// 	if (PlayerRef->PierceTravelPercent <= 1.f)
// 	{
// 		FVector NewActorLocation = PlayerRef->PierceStartPos + (PlayerRef->PierceEndPos - PlayerRef->PierceStartPos) * PlayerRef->PierceTravelPercent;
// 		PlayerRef->SetActorLocation(NewActorLocation, true);
// 		PlayerRef->PierceTravelPercent += 0.05;
//
// 		if (PlayerRef->PierceTravelPercent >= 0.5f)
// 		{
// 			PlayerRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
// 		}
// 	}
// 	else
// 	{
// 		PlayerRef->GetCharacterMovement()->Velocity = PlayerRef->PierceDirection * 1000.f;
// 		ResetPierceValues();
// 		PlayerRef->EnableInput(PlayerController);
// 		GetWorld()->GetTimerManager().ClearTimer(PlayerRef->PierceTravelTimer);
// 	}
// }
//
// void UPlayerBaseState::ResetPierceValues()
// {
// 	PlayerRef->PierceDirection = FVector::Zero();
// 	PlayerRef->PierceStartPos = FVector::Zero();
// 	PlayerRef->PierceEndPos = FVector::Zero();
// 	PlayerRef->ClosestPierceTarget = nullptr;
// 	PlayerRef->PierceTravelPercent = 0.f;
// 	PlayerRef->JumpCurrentCount = 0;
// }
//
// void UPlayerBaseState::ReleasePierceButton()
// {
// 	PlayerRef->bIsPierceButtonReleased = true;
// }
