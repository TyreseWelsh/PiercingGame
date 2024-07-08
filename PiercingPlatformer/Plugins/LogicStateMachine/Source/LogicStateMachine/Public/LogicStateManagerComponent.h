// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LogicStateBase.h"
#include "LogicStateManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOGICSTATEMACHINE_API ULogicStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULogicStateManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine")
	FString InitialState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine")
	TMap<FString, TSubclassOf<ULogicStateBase>> AvailableStates;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine")
	bool bDebug = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<ULogicStateBase*> StateHistory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine")
	int StateHistoryLength = 6;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULogicStateBase> CurrentState;

	UPROPERTY()
	TMap<FString, ULogicStateBase*> StateMap;

	UFUNCTION(Category = "State Machine")
	void SwitchStateByKey(FString NewStateKey);
	UFUNCTION(Category = "State Machine")
	void SwitchStateByValue(ULogicStateBase* NewStateValue);
	
	UFUNCTION(Category = "State Machine")
	void InitStateManager();

private:
	UFUNCTION(Category = "State Machine")
	void SwitchState(ULogicStateBase* NewState);
	
	bool bCanTickState = false;
	void InitializeAvailableStates();
};
