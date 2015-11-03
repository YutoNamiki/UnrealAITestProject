// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "MyTestSceneComponent.h"


// Sets default values for this component's properties
UMyTestSceneComponent::UMyTestSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyTestSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyTestSceneComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UMyTestSceneComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedChainEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject(Component):PostEditChangeChainProperty"));
	Super::PostEditChangeChainProperty(PropertyChangedChainEvent);
}

void UMyTestSceneComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject(Component):PostEditChangeProperty"));
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UMyTestSceneComponent::PostInterpChange(UProperty* PropertyThatChanged)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject(Component):PostInterpChange"));
	Super::PostInterpChange(PropertyThatChanged);
}

void UMyTestSceneComponent::OnComponentDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("UActorComponent:OnComponentDestroyed"));
	Super::OnComponentDestroyed();
}

void UMyTestSceneComponent::OnRegister()
{
	UE_LOG(LogTemp, Warning, TEXT("UActorComponent:OnRegister"));
	Super::OnRegister();
}

void UMyTestSceneComponent::OnAttachmentChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("USceneComponent:OnAttachmentChanged"));
	Super::OnAttachmentChanged();
}

void UMyTestSceneComponent::OnChildAttached(USceneComponent* ChildComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("USceneComponent:OnChildAttached"));
	Super::OnChildAttached(ChildComponent);
}

void UMyTestSceneComponent::OnChildDetached(USceneComponent* ChildComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("USceneComponent:OnChildDetached"));
	Super::OnChildDetached(ChildComponent);
}

void UMyTestSceneComponent::PostEditComponentMove(bool bFinished)
{
	UE_LOG(LogTemp, Warning, TEXT("USceneComponent:PostEditComponentMove"));
	Super::PostEditComponentMove(bFinished);
}

void UMyTestSceneComponent::PreFeatureLevelChange(ERHIFeatureLevel::Type PendingFeatureLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("USceneComponent:PreFeatureLevelChange"));
	Super::PreFeatureLevelChange(PendingFeatureLevel);
}
