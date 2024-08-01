// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemyInfo.h"
#include "FloatingDamageNumber.h"
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

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitEnemyData();
}

void AEnemyBase::PostInitProperties()
{
	Super::PostInitProperties();

	//InitEnemyData();
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//InitEnemyData();

}

void AEnemyBase::TakeDamage_Implementation(float Damage)
{
	EnemyHealth -= Damage;
	
	if(EnemyHealth <= 0)
	{
		KillActor_Implementation(Damage);
		return;
	}

	SpawnDamageNumber_Implementation(Damage, FLinearColor::White);
	
	if(IsValid(HurtAnim))
	{
		MatInstDynamic->SetVectorParameterValue(FName("HurtColour"), FLinearColor::White);
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(HurtAnim, "DefaultSlot", 1.f, 0.f);
	}
}

void AEnemyBase::KillActor_Implementation(float Damage)
{
	SpawnDamageNumber_Implementation(Damage, FLinearColor::Red);
	MatInstDynamic->SetVectorParameterValue(FName("HurtColour"), FLinearColor::Red);
	// When health reaches 0, play death animation (using notifies to flash red)
	// After death animation is finished, will spawn a dead body sprite of the enemy at the correct location
}

void AEnemyBase::StartHurtEffect_Implementation()
{
	MatInstDynamic->SetScalarParameterValue(FName("Hurt"), 1);
}

void AEnemyBase::EndHurtEffect_Implementation()
{
	MatInstDynamic->SetScalarParameterValue(FName("Hurt"), 0);
}

void AEnemyBase::SpawnDamageNumber_Implementation(int _Damage, FLinearColor _TextColour)
{
	FTransform SpawnTransform;
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FVector SpawnScale = FVector::One();
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	SpawnTransform.SetScale3D(SpawnScale);
	
	
	CurrentDamageNumberPopup = GetWorld()->SpawnActor<AFloatingDamageNumber>(DamageNumberPopupClass, SpawnTransform);
	CurrentDamageNumberPopup->Init(_Damage, _TextColour);
}

void AEnemyBase::InitEnemyData()
{
	if(IsValid(EnemyDataAsset))
	{
		// if(GetSprite()->SetFlipbook(EnemyDataAsset->BaseFlipbook))
		// {
		// 	GEngine->AddOnScreenDebugMessage(int32(-1), 3.f, FColor::Green, "Source flipbook set!");
		//
		// }
		// else
		// {
		// 	GEngine->AddOnScreenDebugMessage(int32(-1), 3.f, FColor::Red, "Source flipbook NOT set!");
		// }
		
		GetAnimationComponent()->SetAnimInstanceClass(EnemyDataAsset->PaperAnimBP);
		EnemyName = EnemyDataAsset->Name;
		EnemyHealth = EnemyDataAsset->Health;
		EnemyMoveSpeed = EnemyDataAsset->MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeed = EnemyMoveSpeed;
		EnemyDamage = EnemyDataAsset->MoveSpeed;

		MatInstDynamic = MatInstDynamic->Create(GetSprite()->GetMaterial(0), this, FName("DynamicPixelPerfectMaterial"));
		GetSprite()->SetMaterial(0, MatInstDynamic);
		
		InitAnimations();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(int32(-1), 1.f, FColor::Green, "No data!");
	}
}

void AEnemyBase::InitAnimations()
{
	IdleAnim = EnemyDataAsset->IdleAnim;
	HurtAnim = EnemyDataAsset->HurtAnim;
}
