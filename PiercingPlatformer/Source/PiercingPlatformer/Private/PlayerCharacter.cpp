// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "PaperZDAnimInstance.h"
#include "../Public/BasicAttack.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "../Public/Pierceable.h"



APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Setup Collision Capsule
	GetCapsuleComponent()->InitCapsuleSize(36.f, 48.f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	// Initialisation and setup of PierceRadius Sphere Collider
	PierceRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PierceRadius"));
	PierceRadius->InitSphereRadius(100.f);
	PierceRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PierceRadius->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	PierceRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PierceRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	PierceRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	PierceRadius->bHiddenInGame = false;
	PierceRadius->SetupAttachment(RootComponent);

	// Setup of Character Movement Component
	GetCharacterMovement()->GravityScale = 3.f;
	GetCharacterMovement()->MaxAcceleration = 8192.f;
	GetCharacterMovement()->GroundFriction = 40.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 1200.f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->AirControlBoostMultiplier = 4.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 999999999.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	JumpMaxCount = 2;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		PierceAimDelegate.BindUFunction(this, "AimPierce", PlayerController);
		PierceTravelDelegate.BindUFunction(this, "Pierce", PlayerController);


		PlayerController->SetShowMouseCursor(true);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Run);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);

		EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Started, this, &APlayerCharacter::StartPierce);
		EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndPierce);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("%s Failed to find an Enhanced Input component!"), *GetNameSafe(this);
	}
}

void APlayerCharacter::SwapCamera(AActor* NewViewTarget, float BlendTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetViewTargetWithBlend(NewViewTarget, BlendTime);
}

void APlayerCharacter::StartAttack_Implementation()
{
	if (APlayerController* _PlayerController = Cast<APlayerController>(GetController()))
	{
		// If attack obj already exists, destroy it
		if (BasicAttack != nullptr)
		{
			BasicAttack->Destroy();
		}


		FTransform NewBasicAttackTransform;

		// New Location
		NewBasicAttackTransform.SetLocation(GetActorLocation());

		// New Rotation
		FVector MouseLocation;
		FVector MouseDirection;
		_PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		FRotator NewAttackRotation = UKismetMathLibrary::FindLookAtRotation(FVector(GetActorLocation().X, 0.0f, GetActorLocation().Z), FVector(MouseLocation.X, 0.f, MouseLocation.Z));
		NewBasicAttackTransform.SetRotation(NewAttackRotation.Quaternion());

		// New Scale
		NewBasicAttackTransform.SetScale3D(FVector(1.f, 1.f, 1.f));


		BasicAttack = GetWorld()->SpawnActorDeferred<ABasicAttack>(ABasicAttack::StaticClass(), NewBasicAttackTransform);
	}
}

//void APlayerCharacter::AnimSequencePlaybackComplete(const UPaperZDAnimSequence* InAnimSequence)
//{
//	if (InAnimSequence == BasicAttackAnim)
//	{
//		BasicAttack->Destroy();
//	}
//}

	void APlayerCharacter::EndAttack_Implementation()
	{
		BasicAttack->Destroy();
	}

	void APlayerCharacter::Run(const FInputActionValue& Value)
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), Value.Get<float>());
}

//void APlayerCharacter::Jump(const FInputActionValue& Value)
//{
//}

void APlayerCharacter::Attack()
{
	if (APlayerController* _PlayerController = Cast<APlayerController>(GetController()))
	{
		UE_LOG(LogTemp, Error, TEXT("ATTACK!"));
		GetAnimInstance()->PlayAnimationOverride(BasicAttackAnim, "DefaultSlot", 1.f, 0.f);
	}

	//if (APlayerController* _PlayerController = Cast<APlayerController>(GetController()))
	//{
	//	// If attack obj already exists, destroy it
	//	if (BasicAttack != nullptr)
	//	{
	//		BasicAttack->Destroy();
	//	}


	//	FTransform NewBasicAttackTransform;

	//	// New Location
	//	NewBasicAttackTransform.SetLocation(GetActorLocation());

	//	// New Rotation
	//	FVector MouseLocation;
	//	FVector MouseDirection;
	//	_PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	//	FRotator NewAttackRotation = UKismetMathLibrary::FindLookAtRotation(FVector(GetActorLocation().X, 0.0f, GetActorLocation().Z), FVector(MouseLocation.X, 0.f, MouseLocation.Z));
	//	NewBasicAttackTransform.SetRotation(NewAttackRotation.Quaternion());

	//	// New Scale
	//	NewBasicAttackTransform.SetScale3D(FVector(1.f, 1.f, 1.f));


	//	BasicAttack = GetWorld()->SpawnActorDeferred<ABasicAttack>(ABasicAttack::StaticClass(), NewBasicAttackTransform);

	//	GetAnimInstance()->OnAnimSequencePlaybackComplete(BasicAttackAnim);
	//	//GetAnimInstance()->PlayAnimationOverride(BasicAttackAnim, "DefaultSlot", 1.f, 0.f, FZDOnAnimationOverrideEnd);
	//}
}

void APlayerCharacter::StartPierce()
{
	TSet<AActor*> PierceableTargets;
	PierceRadius->GetOverlappingActors(PierceableTargets);

	float DistanceToClosestTarget = 9999.f;

	for (AActor* target : PierceableTargets)
	{
		if (IPierceable* PiercableInterface = Cast<IPierceable>(target))
		{
			if (PiercableInterface != nullptr)
			{
				float distanceToTarget = FVector::Distance(target->GetActorLocation(), GetActorLocation());

				if (distanceToTarget < DistanceToClosestTarget)
				{
					DistanceToClosestTarget = distanceToTarget;
					ClosestPierceTarget = target;
				}
			}
		}
	}

	if (ClosestPierceTarget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NO TARGETS"));
	}
	else
	{
		GetWorldSettings()->SetTimeDilation(0.1f);

		//GetWorld()->GetTimerManager().SetTimer(PierceAimTimer, this, &APlayerCharacter::AimPierce, 0.002f, true);
		GetWorld()->GetTimerManager().SetTimer(PierceAimTimer, PierceAimDelegate, 0.002f, true);
	}

}

void APlayerCharacter::AimPierce(APlayerController* _PlayerController)
{
	FVector MouseLocation;
	FVector MouseDirection;
	_PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

	FVector VectorFromTargetToMouse = FVector(ClosestPierceTarget->GetActorLocation().X, 0.0f, ClosestPierceTarget->GetActorLocation().Z) - FVector(MouseLocation.X, 0.f, MouseLocation.Z);
	
	VectorFromTargetToMouse.Normalize();
	PierceDirection = VectorFromTargetToMouse * -1;
	PierceEndPos = ClosestPierceTarget->GetActorLocation() + (PierceDirection * PierceDistance);

	PierceStartPos = ClosestPierceTarget->GetActorLocation() + (PierceDirection * -1 * PierceDistance);
}

void APlayerCharacter::EndPierce()
{
	if (ClosestPierceTarget != nullptr)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		if (APlayerController* _PlayerController = Cast<APlayerController>(GetController()))
		{
			DisableInput(_PlayerController);
			GetCharacterMovement()->Velocity = FVector(0, 0, 0);
			SetActorLocation(PierceStartPos, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
			GetWorld()->GetTimerManager().ClearTimer(PierceAimTimer);
			GetWorldSettings()->SetTimeDilation(1.f);
			GetWorld()->GetTimerManager().SetTimer(PierceTravelTimer, PierceTravelDelegate, 0.01f, true);
		}
	}
}

void APlayerCharacter::Pierce(APlayerController* _PlayerController)
{
	if (PierceTravelPercent <= 1.f)
	{
		FVector NewActorLocation = PierceStartPos + (PierceEndPos - PierceStartPos) * PierceTravelPercent;
		SetActorLocation(NewActorLocation, true);
		PierceTravelPercent += 0.05;

		if (PierceTravelPercent >= 0.5f)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		}
	}
	else
	{
		GetCharacterMovement()->Velocity = PierceDirection * 1000.f;
		ResetPierceValues();
		EnableInput(_PlayerController);
		GetWorld()->GetTimerManager().ClearTimer(PierceTravelTimer);
	}
}

void APlayerCharacter::ResetPierceValues()
{
	PierceDirection = FVector::Zero();
	PierceStartPos = FVector::Zero();
	PierceEndPos = FVector::Zero();
	ClosestPierceTarget = nullptr;
	PierceTravelPercent = 0.f;
	JumpCurrentCount = 0;
}