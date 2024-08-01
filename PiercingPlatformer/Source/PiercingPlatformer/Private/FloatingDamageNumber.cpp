// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingDamageNumber.h"

#include "MovieSceneSequenceID.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AFloatingDamageNumber::AFloatingDamageNumber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
	
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AFloatingDamageNumber::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFloatingDamageNumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloatingDamageNumber::Init(int NewDamageAmount, FLinearColor NewTextColor, float NewMoveDistance, float NewMoveSpeed)
{
	DamageAmount = NewDamageAmount;
	TextColor = NewTextColor;
	InitWidgetValues();

	MoveDistance = NewMoveDistance;
	MoveSpeed = NewMoveSpeed;
	
	FindEndPosition();
	MoveTimerDelegate.BindUFunction(this, "LerpPosition");
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, MoveTimerDelegate, MoveSpeed, true);
}

void AFloatingDamageNumber::FindEndPosition()
{
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if(PlayerCharacter)
	{
		BaseStartPosition = GetActorLocation();
		PlayerDirection = PlayerCharacter->GetActorLocation() - BaseStartPosition;
		PlayerDirection.Normalize();

		BaseEndPosition = ((PlayerDirection * MoveDistance) * -1) + GetActorLocation();

		float RotationOffset = FMath::RandRange(MinRotationOffset, MaxRotationOffset);
		AlteredEndPosition = BaseEndPosition.RotateAngleAxis(RotationOffset, FVector(0,1,0));

		FVector StartToAlteredEndVector = AlteredEndPosition - BaseStartPosition;
		float StartPosOffset = FMath::RandRange(MinStartPosOffset, MaxStartPosOffset);
		AlteredStartPosition = BaseStartPosition + (StartToAlteredEndVector * StartPosOffset);
	}
}

void AFloatingDamageNumber::LerpPosition()
{
	if(LerpAlpha <= 1.f)
	{
		LerpAlpha += 0.025f;
		
		FVector NewLocation = FMath::Lerp(AlteredStartPosition, AlteredEndPosition, LerpAlpha);
		SetActorLocation(NewLocation);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		Destroy();
	}
}