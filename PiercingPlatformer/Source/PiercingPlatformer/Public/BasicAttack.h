// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicAttack.generated.h"

class USceneComponent;
class UBoxComponent;

UCLASS()
class PIERCINGPLATFORMER_API ABasicAttack : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* PivotPoint;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackCollider;
	
public:	
	// Sets default values for this actor's properties
	ABasicAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	float AttackLength = 92.f;
};
