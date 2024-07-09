// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicStateManagerComponent.h"

// Sets default values for this component's properties
ULogicStateManagerComponent::ULogicStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULogicStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeAvailableStates();
}


// Called every frame
void ULogicStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bCanTickState)
	{
		CurrentState->OnStateTick();
	}
	if(bDebug)
	{
		GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, this->GetOwner()->GetName() + " - Current State: " + CurrentState->StateDebugName.ToString());
	}
}

// EXAMPLE: InitialSpawning state - non looping
void ULogicStateManagerComponent::SwitchStateByKey(FString NewStateKey)
{
	if(ULogicStateBase* NewState = StateMap.FindRef(NewStateKey))
	{
		if(!CurrentState)
		{
			CurrentState = NewState;
			SwitchState(NewState);
			return;
		}
		
		if(CurrentState->GetClass() == NewState->GetClass() && CurrentState->bCanStateLoop == false)
		{
			// Switching to same state and non Loopable
			if(bDebug)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red)
			}
			// Will return and continue with current state code without swapping state
			return;
		}

		SwitchState(NewState);
	}
}

void ULogicStateManagerComponent::SwitchStateByValue(ULogicStateBase* NewStateValue)
{
}

void ULogicStateManagerComponent::InitStateManager()
{
	SwitchStateByKey(InitialState);
}

void ULogicStateManagerComponent::SwitchState(ULogicStateBase* NewState)
{
	// Switching to same state and loopable or switching to different state
	bCanTickState = false;
	CurrentState->OnStateExit();

	if(StateHistory.Num() >= StateHistoryLength) // NOTE: Can probably change this to greater than instead, if true RemoveAt(0) then after the if statement, Push
	{
		StateHistory.RemoveAt(0);
	}
	StateHistory.Push(CurrentState);

	CurrentState = NewState;

		
	// Starting new current state
	CurrentState->OnStateEnter(GetOwner());
	bCanTickState = true;
}

void ULogicStateManagerComponent::InitializeAvailableStates()
{
	// Iterates over all available states provided and constructs a new LogicStateBase object for each, and adds it to our State Map
	for(auto It = AvailableStates.CreateConstIterator(); It; ++It)
	{
		ULogicStateBase* NewState = NewObject<ULogicStateBase>(this, It->Value);
		StateMap.Add(It->Key, NewState);
	}
}

