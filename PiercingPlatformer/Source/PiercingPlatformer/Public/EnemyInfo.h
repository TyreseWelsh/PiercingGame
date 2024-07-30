// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyInfo.generated.h"


class UPaperFlipbook;
class UPaperZDAnimInstance;

/**
 * 
 */
UCLASS(BlueprintType)
class PIERCINGPLATFORMER_API UEnemyInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* BaseFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPaperZDAnimInstance> PaperAnimBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Damage;
};
