// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicAttack.generated.h"

class USceneComponent;
class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;
class APlayerCharacter;

UCLASS()
class PIERCINGPLATFORMER_API ABasicAttack : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* PivotPoint;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperFlipbookComponent> AttackSprite;

	UPROPERTY(EditDefaultsOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> AttackHit;

	AActor* AttackHitActor;
	
public:	
	// Sets default values for this actor's properties
	ABasicAttack();

	UPaperFlipbookComponent* GetSprite() { return AttackSprite; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbook* AttackSourceFlipBook;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwningPlayer(APlayerCharacter* NewOwner);
	void InAirHitBounce();
	void SpawnHitEffect(AActor* OtherActor);

	UBoxComponent* GetCollider() { return AttackCollider; }

private:
	float AttackLength = 92.f;
	int AttackDamage = 10;

	APlayerCharacter* OwningPlayer;
	bool bJumpHasReset = false;

	TArray<AActor*> HitActors;
};
