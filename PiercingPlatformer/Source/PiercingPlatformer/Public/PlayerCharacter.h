// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "../Public/CanAttack.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPaperZDAnimSequence;

class ABasicAttack;

UCLASS()
class PIERCINGPLATFORMER_API APlayerCharacter : public APaperZDCharacter, public ICanAttack
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PierceRadius;


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

public:
	UFUNCTION(BlueprintCallable)
	void SwapCamera(AActor* NewViewTarget, float BlendTime);


	// ICanAttack functions
	void StartAttack_Implementation();
	void EndAttack_Implementation();

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


private:
	bool bIsRunning = false;

	// Piercing
	AActor* ClosestPierceTarget;
	float PierceForce = 3000.f;
	FVector PierceDirection;
	FVector PierceStartPos;
	FVector PierceEndPos;
	float PierceTravelPercent;
	float PierceDistance = 250.f;
	FTimerHandle PierceAimTimer;
	FTimerDelegate PierceAimDelegate;
	FTimerHandle PierceTravelTimer;
	FTimerDelegate PierceTravelDelegate;
	bool bIsPiercing = false;

	// Attacking
	ABasicAttack* BasicAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Attacking, meta = (AllowPrivateAccess))
	UPaperZDAnimSequence* BasicAttackAnim;


};
