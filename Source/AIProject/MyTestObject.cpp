// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "MyTestObject.h"
#include "Engine/World.h"

void UMyTestObject::Function(UWorld* world)
{
	auto location1 = FVector(-320, 20, 480);
	auto location2 = FVector(390, -30, -300);
	if (world->LineTraceTestByChannel(location1, location2, ECollisionChannel::ECC_WorldStatic))
	{
		UE_LOG(LogTemp, Warning, TEXT("LineTrace Hit Now!!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LineTrace No Hit..."));
	}
}
