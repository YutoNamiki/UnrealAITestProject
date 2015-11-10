// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "TimeCounterComponent.h"


// Sets default values for this component's properties
UTimeCounterComponent::UTimeCounterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTimeCounterComponent::BeginPlay()
{
	Super::BeginPlay();

	startTimes.Init(0, CounterCount);
	endTimes.Init(0, CounterCount);
}


// Called every frame
void UTimeCounterComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}

void UTimeCounterComponent::Start(int32 index)
{
	startTimes[index] = FDateTime::Now();
	endTimes[index] = startTimes[index];
}

void UTimeCounterComponent::End(int32 index)
{
	endTimes[index] = FDateTime::Now();
}

void UTimeCounterComponent::Display(int32 index)
{
	auto deltaTime = endTimes[index] - startTimes[index];
	auto measurementTimeString = FString("[") + FString::FromInt(index) + FString("] Œv‘ªŽžŠÔF ") + FString::FromInt(deltaTime.GetMilliseconds()) + FString(" ms");
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, measurementTimeString);
}

int32 UTimeCounterComponent::GetElapsedTimeFromStart(int32 index)
{
	auto currentTime = FDateTime::Now();
	return (currentTime - startTimes[index]).GetMilliseconds();
}
