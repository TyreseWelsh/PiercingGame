// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "../Public/CanAttack.h"
#include "../Public/Damageable.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class UBoxComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPaperZDAnimSequence;

class ABasicAttack;


//UENUM(BlueprintType)
//enum EPlayerStatus
//{
//	Idle			UMETA(DisplayName = "Idle"),
//	Running			UMETA(DisplayName = "Running"),
//	Jumping			UMETA(DisplayName = "Jumping"),
//	Piercing		UMETA(DisplayName = "Piercing"),
//	Attacking		UMETA(DisplayName = "Attacking"),
//	WallJumping		UMETA(DisplayName = "Wall Jumping"),
//};


UCLASS()
class PIERCINGPLATFORMER_API APlayerCharacter : public APaperZDCharacter, public IDamageable, public ICanAttack
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PierceRadius;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LeftWallCollider;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWallCollider;


public:
	APlayerCharacter();
	

private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PierceAction;


protected:
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UFUNCTION(BlueprintCallable)
	void SwapCamera(AActor* NewViewTarget, float BlendTime);

	virtual void Jump() override;
	virtual void StopJumping() override;


	void TakeDamage_Implementation(int Damage);

	// ICanAttack functions
	void StartAttack_Implementation();
	void EndAttack_Implementation();
	void EnableAttackCollider_Implementation();
	void DisableAttackCollider_Implementation();

private:
	void Run(const FInputActionValue& Value);

	//void Jump(const FInputActionValue& Value);		// Will be a custom jump function overriding the base implementation

	void Attack();


	//UFUNCTION(Category = Pierce)
	void StartPierce();
	
	UFUNCTION()
	void AimPierce(APlayerController* _PlayerController);

	//UFUNCTION(Category = Pierce)
	void EndPierce();

	UFUNCTION()
	void Pierce(APlayerController* _PlayerController);

	//UFUNCTION(Category = Pierce)
	void ResetPierceValues();

	void ReleasePierceButton();


private:

	bool bIsRunning = false;
	bool bIsInAir = false;

	int Health = 100;

	// Piercing
	AActor* ClosestPierceTarget;
	float PierceForce = 3000.f;
	FVector PierceDirection;
	FVector PierceStartPos;
	FVector PierceEndPos;
	float PierceTravelPercent;
	float PierceDistance = 350.f;
	float PierceDistanceMultiplier = 1.f;
	FTimerHandle PierceAimTimer;
	FTimerDelegate PierceAimDelegate;
	FTimerHandle PierceTravelTimer;
	FTimerDelegate PierceTravelDelegate;
	bool bIsPiercing = false;

	float PierceStartDelay = 0.f;
	float MAX_PIERCE_START_DELAY = 0.025f;
	bool bIsPierceButtonReleased = true;

	// Attacking
	ABasicAttack* BasicAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	UPaperZDAnimSequence* BasicAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABasicAttack> AttackToSpawn;

	
};
