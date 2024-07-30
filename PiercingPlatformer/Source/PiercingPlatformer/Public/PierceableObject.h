// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/Pierceable.h"
#include "../Public/Damageable.h"
#include "PierceableObject.generated.h"

class USphereComponent;
class UPaperFlipbookComponent;

UCLASS()
class PIERCINGPLATFORMER_API APierceableObject : public AActor, public IPierceable, public IDamageable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	USphereComponent* Collider;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	UPaperFlipbookComponent* PierceEmblem;

public:	
	// Sets default values for this actor's properties
	APierceableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void TakeDamage_Implementation(int Damage);
	void KillActor_Implementation() override;
};
