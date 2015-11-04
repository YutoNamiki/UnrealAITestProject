// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TimeCounterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UTimeCounterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeCounter")
	int32 CounterCount = 1;

	// Sets default values for this component's properties
	UTimeCounterComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "TimeCounter")
	void Start(int32 index);

	UFUNCTION(BlueprintCallable, Category = "TimeCounter")
	void End(int32 index);

	UFUNCTION(BlueprintCallable, Category = "TimeCounter")
	void Display(int32 index);

private:

	TArray<FDateTime> startTimes;
	TArray<FDateTime> endTimes;
	
};
