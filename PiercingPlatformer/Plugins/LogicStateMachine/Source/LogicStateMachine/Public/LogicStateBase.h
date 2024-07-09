// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LogicStateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LOGICSTATEMACHINE_API ULogicStateBase : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName StateDebugName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStateTick = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStateLoop = false;


	virtual void OnStateEnter(AActor* StateOwner);
	virtual void OnStateTick();
	virtual void OnStateExit();
};
