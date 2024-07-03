// Fill out your copyright notice in the Description page of Project Settings.


#include "PierceableObject.h"
#include "Components/SphereComponent.h"

// Sets default values
APierceableObject::APierceableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->InitSphereRadius(26.f);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = Collider;

	TempSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	TempSphere->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	TempSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TempSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APierceableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APierceableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APierceableObject::TakeDamage_Implementation(int Damage)
{

}

