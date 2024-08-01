// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingDamageNumber.generated.h"

class UWidgetComponent;
class UUserWidget;

UCLASS()
class PIERCINGPLATFORMER_API AFloatingDamageNumber : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* Widget;
public:	
	// Sets default values for this actor's properties
	AFloatingDamageNumber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Init(int NewDamageAmount, FLinearColor NewTextColor, float NewMoveDistance = 100.f, float NewMoveSpeed = 0.01f);
	UFUNCTION(BlueprintImplementableEvent)
	void InitWidgetValues();
	
	TSubclassOf<UUserWidget> DamageNumberWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor TextColor;
	
	float MoveDistance;
	float MoveSpeed;

private:
	void FindEndPosition();
	UFUNCTION()
	void LerpPosition();

	TObjectPtr<ACharacter> PlayerCharacter;
	
	FTimerHandle MoveTimerHandle;
	FTimerDelegate MoveTimerDelegate;
	float LerpAlpha = 0.f;
	
	FVector BaseStartPosition;
	FVector AlteredStartPosition;
	FVector PlayerDirection;
	FVector BaseEndPosition;
	FVector AlteredEndPosition;
	
	float MinRotationOffset = -2.f;
	float MaxRotationOffset = 2.f;
	float MinStartPosOffset = 0.2f;
	float MaxStartPosOffset = 0.4f;
};
