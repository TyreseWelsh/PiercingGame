// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Damageable.h"
#include "EnemyBase.generated.h"

class UEnemyInfo;
class UPaperZDAnimSequence;
class AFloatingDamageNumber;

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API AEnemyBase : public APaperZDCharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void Tick(float DeltaSeconds) override;

	// Data variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionInfo")
	UEnemyInfo* EnemyDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstructionInfo")
	FString EnemyName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstructionInfo")
	int EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstructionInfo")
	float EnemyMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstructionInfo")
	int EnemyDamage;


	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UPaperZDAnimSequence* IdleAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UPaperZDAnimSequence* HurtAnim;


	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Other")
	TSubclassOf<AFloatingDamageNumber> DamageNumberPopupClass;
	
	
	virtual void TakeDamage_Implementation(float Damage) override;
	virtual void KillActor_Implementation(float Damage) override;
	virtual void StartHurtEffect_Implementation() override;
	virtual void EndHurtEffect_Implementation() override;
	UFUNCTION()
	virtual void SpawnDamageNumber_Implementation(int _Damage, FLinearColor _TextColour) override;

private:
	UFUNCTION()
	void InitEnemyData();
	UFUNCTION()
	void InitAnimations();

	UPROPERTY()
	UMaterialInstanceDynamic* MatInstDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<AFloatingDamageNumber> CurrentDamageNumberPopup; 
};
