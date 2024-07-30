// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Damageable.h"
#include "EnemyBase.generated.h"

class UEnemyInfo;
class UPaperZDAnimSequence;

/**
 * 
 */
UCLASS()
class PIERCINGPLATFORMER_API AEnemyBase : public APaperZDCharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void PostInitProperties() override;

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


	virtual void TakeDamage_Implementation(int Damage) override;
	virtual void KillActor_Implementation() override;

private:
	void InitEnemyData();


	UPROPERTY(EditDefaultsOnly, Category = "OneShots", meta = (AllowPrivateAccess = "true"))
	UPaperZDAnimSequence* HurtAnim;
};
