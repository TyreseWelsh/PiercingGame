// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanAttack.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanAttack : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIERCINGPLATFORMER_API ICanAttack
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndAttack();
};
