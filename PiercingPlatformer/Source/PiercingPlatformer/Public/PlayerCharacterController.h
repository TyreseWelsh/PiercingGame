// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// Declaring delegate (event) signatures for in-game player input
DECLARE_MULTICAST_DELEGATE_OneParam(FMoveSignature, const FInputActionValue&);
DECLARE_MULTICAST_DELEGATE(FJumpSignature);
DECLARE_MULTICAST_DELEGATE(FReleaseJumpSignature);
DECLARE_MULTICAST_DELEGATE(FAttackSignature);
DECLARE_MULTICAST_DELEGATE(FPierceSignature);
DECLARE_MULTICAST_DELEGATE(FReleasePierceSignature);



/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	FMoveSignature* GetMoveDelegate();
	FJumpSignature* GetJumpDelegate();
	FReleaseJumpSignature* GetReleaseJumpDelegate();
	FAttackSignature* GetAttackDelegate();
	FPierceSignature* GetPierceDelegate();
	FReleasePierceSignature* GetReleasePierceDelegate();

private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = Input, meta=(AllowPrivateAccess))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category= Input, meta = (AllowPrivateAccess))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* PierceAction;


	void PressMove(const FInputActionValue& Value);
	void PressJump();
	void ReleaseJump();
	void PressAttack();
	void PressPierce();
	void ReleasePierce();
	
private:
	FMoveSignature MoveDelegate;
	FJumpSignature JumpDelegate;
	FReleaseJumpSignature ReleaseJumpDelegate;
	FAttackSignature AttackDelegate;
	FPierceSignature PierceDelegate;
	FReleasePierceSignature ReleasePierceDelegate;
};
