// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/Pierceable.h"
#include "PierceableObject.generated.h"

class USphereComponent;

UCLASS()
class PIERCINGPLATFORMER_API APierceableObject : public AActor, public IPierceable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	USphereComponent* Collider;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	UStaticMeshComponent* TempSphere;

public:	
	// Sets default values for this actor's properties
	APierceableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
