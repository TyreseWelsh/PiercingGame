// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttack.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABasicAttack::ABasicAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = PivotPoint;

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->InitBoxExtent(FVector(AttackLength, 10.f, 32.f));
	AttackCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AttackCollider->SetupAttachment(RootComponent);
	AttackCollider->SetRelativeLocation(FVector(AttackLength, 0.f, 0.f));
	AttackCollider->bHiddenInGame = false;
	AttackCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasicAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}