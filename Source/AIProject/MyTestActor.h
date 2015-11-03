// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyTestActor.generated.h"

UCLASS()
class AIPROJECT_API AMyTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UBoxComponent* BoxCollision;

	// Sets default values for this actor's properties
	AMyTestActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// UObject Functions
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;
	virtual void PostEditImport() override;
	virtual void PostEditUndo(TSharedPtr<ITransactionObjectAnnotation> TransactionAnnotation) override;
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void PostNetReceive() override;
	virtual void PreEditChange(UProperty* PropertyAbountToChange) override;
	virtual void PreEditUndo() override;
	virtual void PreNetReceive() override;

	// AActor Functions
	virtual void PostEditUndo() override;
	virtual void EditorApplyMirror(const FVector& MirrorScale, const FVector& PivotLocation) override;
	virtual void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorKeyPressed(FKey Key, EInputEvent Event) override;
	virtual void EditorReplacedActor(AActor* OldActor) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnSerializeNewActor(FOutBunch& OutBunch) override;
	virtual void PostActorCreated() override;
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostNetInit() override;
	virtual void PostNetReceiveLocationAndRotation() override;
	virtual void PostNetReceivePhysicState() override;
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;
	virtual void PostRegisterAllComponents() override;
	virtual void PostUnregisterAllComponents() override;
	virtual void PreInitializeComponents() override;

	// AActor Events
	UFUNCTION()
	void OnTestActorBeginOverlap(AActor* OtherActor);
	UFUNCTION()
	void OnTestActorEndOverlap(AActor* OtherActor);
	UFUNCTION()
	void OnTestActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnTestBeginCursorOver();
	UFUNCTION()
	void OnTestClicked();

	// UPrimitiveComponent Events
	UFUNCTION()
	void OnUPrimitiveComponentClicked(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void OnUPrimitiveComponentBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnUPrimitiveComponentEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex);
};
