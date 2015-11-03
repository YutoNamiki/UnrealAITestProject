// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "MyTestSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UMyTestSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyTestSceneComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// UObject Functions
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedChainEvent) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostInterpChange(UProperty* PropertyThatChanged) override;
	
	// UActorComponent Functions
	virtual void OnComponentDestroyed() override;
	virtual void OnRegister() override;

	// USceneComponent Function
	virtual void OnAttachmentChanged() override;
	virtual void OnChildAttached(USceneComponent* ChildComponent) override;
	virtual void OnChildDetached(USceneComponent* ChildComponent) override;
	virtual void PostEditComponentMove(bool bFinished) override;
	virtual void PreFeatureLevelChange(ERHIFeatureLevel::Type PendingFeatureLevel) override;

};
