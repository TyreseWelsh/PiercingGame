// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "../Public/CanAttack.h"
#include "../Public/Damageable.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class UBoxComponent;
class ULogicStateManagerComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPaperZDAnimSequence;

class ABasicAttack;


UCLASS()
class PIERCINGPLATFORMER_API APlayerCharacter : public APaperZDCharacter, public IDamageable, public ICanAttack
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PierceRadius;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LeftWallCollider;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWallCollider;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	ULogicStateManagerComponent* LogicStateManagerComponent;


public:
	APlayerCharacter();
	

private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	//
	// UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* RunAction;
	//
	// UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* JumpAction;
	//
	// UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* AttackAction;
	//
	// UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* PierceAction;



protected:
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UFUNCTION()
	USphereComponent* GetPierceRadiusComponent() { return PierceRadius; }
	// UFUNCTION()
	// UBoxComponent* GetLeftWallCollider() { return LeftWallCollider; }
	UFUNCTION()
	UBoxComponent* GetLeftWallCollider() { return LeftWallCollider; }
	UFUNCTION()
	UBoxComponent* GetRightWallCollider() { return RightWallCollider; }
	
	UFUNCTION()
	ULogicStateManagerComponent* GetLogicStateManagerComponent() { return LogicStateManagerComponent; }
	
	UFUNCTION(BlueprintCallable)
	void SwapCamera(AActor* NewViewTarget, float BlendTime);

	// virtual void Jump() override;
	// virtual void StopJumping() override;

	// Interface Implementations
	void TakeDamage_Implementation(int Damage);

	// ICanAttack functions
	void StartAttack_Implementation();
	void EndAttack_Implementation();
	void EnableAttackCollider_Implementation();
	void DisableAttackCollider_Implementation();
	
	void Run(const FInputActionValue& Value);

	//void Jump(const FInputActionValue& Value);		// Will be a custom jump function overriding the base implementation

	void Attack();
	

	bool bIsRunning = false;
	// bIsRunningForward is strictly for animation and is used in the anim BP
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsRunningForward = true;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentMovementDirection;
	bool bIsInAir = false;

	int Health = 100;

	bool bIsJumpBuffered = false;
	bool bIsWallOnRight = false;
	
	// Piercing
	UPROPERTY()
	TObjectPtr<AActor> ClosestPierceTarget;
	float PierceForce = 3000.f;
	FVector PierceDirection;
	FVector PierceStartPos;
	FVector PierceEndPos;
	float PierceTravelPercent;
	float PierceDistance = 400.f;
	float PierceDistanceMultiplier = 1.f;
	bool bIsPiercing = false;

	float PierceStartDelay = 0.f;
	float MAX_PIERCE_START_DELAY = 0.025f;
	bool bIsPierceButtonReleased = true;

	UPROPERTY()
	TObjectPtr<AActor> PierceAimHead;
	UPROPERTY()
	TObjectPtr<AActor> PierceAimTail;
	
	UPROPERTY(EditDefaultsOnly, Category = Piercing, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> PierceHeadBP;

	UPROPERTY(EditDefaultsOnly, Category = Piercing, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> PierceTailBP;
	
	
	// Melee Attacking
	UPROPERTY()
	TObjectPtr<ABasicAttack> BasicAttack;

	UPROPERTY(EditDefaultsOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	UPaperZDAnimSequence* BasicAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABasicAttack> AttackToSpawn;

	// Ranged Attacking
	UPROPERTY()
	TObjectPtr<AActor> RangedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Ranged Attacking", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> RangedWeaponToSpawn;
};
