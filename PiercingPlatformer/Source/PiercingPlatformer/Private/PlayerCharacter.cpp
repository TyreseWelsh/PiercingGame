// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "PaperZDAnimInstance.h"
#include "../Public/BasicAttack.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "../Public/Pierceable.h"


#include "DrawDebugHelpers.h"



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

	// Left wall jump collider
	LeftWallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWallCollider"));
	LeftWallCollider->SetRelativeLocation(FVector(-35.f, 0.f, 0.f));
	LeftWallCollider->InitBoxExtent(FVector(4.f, 32.f, 46.f));
	LeftWallCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftWallCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftWallCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftWallCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	LeftWallCollider->SetupAttachment(RootComponent);

	// Right wall jump collider
	RightWallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWallCollider"));
	RightWallCollider->SetRelativeLocation(FVector(35.f, 0.f, 0.f));
	RightWallCollider->InitBoxExtent(FVector(4.f, 32.f, 46.f));
	RightWallCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightWallCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightWallCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightWallCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	RightWallCollider->SetupAttachment(RootComponent);

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

		LeftWallCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

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
		EnhancedInputComponent->BindAction(PierceAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReleasePierceButton);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("%s Failed to find an Enhanced Input component!"), *GetNameSafe(this);
	}
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OverlappedComponent == LeftWallCollider)
	//{
	//	if ()
	//}
	//else if (OverlappedComponent == RightWallCollider)
	//{

	//}
}

void APlayerCharacter::SwapCamera(AActor* NewViewTarget, float BlendTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetViewTargetWithBlend(NewViewTarget, BlendTime);
}

void APlayerCharacter::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}

void APlayerCharacter::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
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
		GetAnimInstance()->PlayAnimationOverride(BasicAttackAnim, "DefaultSlot", 1.f, 0.f);
	}
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
		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "Start pierce");


		bIsPierceButtonReleased = false;

		GetWorldSettings()->SetTimeDilation(0.f);

		GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);

		GetWorld()->GetTimerManager().SetTimer(PierceAimTimer, PierceAimDelegate, 0.001f * GetWorld()->DeltaRealTimeSeconds, true);
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

	PierceDistanceMultiplier -= GetWorld()->DeltaRealTimeSeconds * 8;
	if (PierceDistanceMultiplier <= 0.3f)															// Distance multiplier was 0.4 but has been divided by 10 to account for delta times small values
	{
		// NOTE: Ideally exit the pierce state which will cancel the pierce all together easily
		ClosestPierceTarget = nullptr;
		EndPierce();
		return;
	}

	// Pierce distance multiplied by PierceDistanceMultiplier so the distance covered is continuously reduced while the player holds the button, incentivising faster gameplay
	FVector PiercePower = (PierceDirection * PierceDistance) * PierceDistanceMultiplier;
	PierceEndPos = ClosestPierceTarget->GetActorLocation() + PiercePower;
	PierceStartPos = ClosestPierceTarget->GetActorLocation() + (-1 * PiercePower);


	PierceStartDelay += GetWorld()->DeltaRealTimeSeconds;
	if (PierceStartDelay >= MAX_PIERCE_START_DELAY && bIsPierceButtonReleased)
	{
		EndPierce();
	}


	GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "Aiming");

	DrawDebugLine(GetWorld(), ClosestPierceTarget->GetActorLocation(), PierceEndPos, FColor::Red);
	DrawDebugLine(GetWorld(), ClosestPierceTarget->GetActorLocation(), PierceStartPos, FColor::Cyan);
}

void APlayerCharacter::EndPierce()
{
	GetWorldSettings()->SetTimeDilation(1.f);

	if (ClosestPierceTarget != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "End pierce");

		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		if (APlayerController* _PlayerController = Cast<APlayerController>(GetController()))
		{
			DisableInput(_PlayerController);
			GetCharacterMovement()->Velocity = FVector(0, 0, 0);
			SetActorLocation(PierceStartPos, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);

			GetWorld()->GetTimerManager().SetTimer(PierceTravelTimer, PierceTravelDelegate, 0.01f, true);
		}
	}

	if (PierceAimTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(PierceAimTimer);
	}
	PierceDistanceMultiplier = 1.f;
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

void APlayerCharacter::ReleasePierceButton()
{
	bIsPierceButtonReleased = true;
}
