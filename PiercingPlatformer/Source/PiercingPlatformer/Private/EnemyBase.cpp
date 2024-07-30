// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemyInfo.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
	GetCapsuleComponent()->InitCapsuleSize(32.f, 46.f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetSprite()->SetRelativeLocation(FVector(0, 0, -50.f));

	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 999999999.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
}

void AEnemyBase::PostInitProperties()
{
	Super::PostInitProperties();

	InitEnemyData();
}

void AEnemyBase::TakeDamage_Implementation(int Damage)
{
	EnemyHealth -= Damage;

	if(EnemyHealth <= 0)
	{
		KillActor_Implementation();
		return;
	}

	if(IsValid(HurtAnim))
	{
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(HurtAnim, "DefaultSlot", 1.f, 0.f);
	}
}

void AEnemyBase::KillActor_Implementation()
{
	Destroy();
}

void AEnemyBase::InitEnemyData()
{
	if(IsValid(EnemyDataAsset))
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "Valid data!");
		
		if(GetSprite()->SetFlipbook(EnemyDataAsset->BaseFlipbook))
		{
			GEngine->AddOnScreenDebugMessage(int32(-1), 3.f, FColor::Green, "Source flipbook set!");

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(int32(-1), 3.f, FColor::Red, "Source flipbook NOT set!");

		}
		
		GetAnimationComponent()->SetAnimInstanceClass(EnemyDataAsset->PaperAnimBP);
		EnemyName = EnemyDataAsset->Name;
		EnemyHealth = EnemyDataAsset->Health;
		EnemyMoveSpeed = EnemyDataAsset->MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeed = EnemyMoveSpeed;
		EnemyDamage = EnemyDataAsset->MoveSpeed;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "No data!");
	}
}
