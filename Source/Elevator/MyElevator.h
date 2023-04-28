// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyElevator.generated.h"

UCLASS()
class ELEVATOR_API AMyElevator : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyElevator();
	float CurveTime;
	UFUNCTION()
		void TimelineProgress(float Value);
	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* Root;
	//Base mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* Base;
	//Doors' mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* LDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorColl")
		UBoxComponent* L_Coll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* RDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorColl")
		UBoxComponent* R_Coll;
	//Collison for trigering with dors and with up and down of mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collisions")
		class UBoxComponent* Trig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collisions")
		class UBoxComponent* Base_Trig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collisions")
		class UBoxComponent* TrigUp;

	UPROPERTY(EditAnywhere)
		UCurveFloat* CurveFloat;
	UPROPERTY(EditAnywhere)
		UCurveFloat* OpenCurve;
	UPROPERTY(EditAnywhere)
		UCurveFloat* CloseCurve;

	UPROPERTY()
		FRotator StartLoc;
	UPROPERTY()
		FRotator EndLoc;
	UPROPERTY(Editanywhere, Category = "Timeline")
		float YOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OnOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OnOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void StartOpening();
	bool bIsOpen;
	bool bIsClose;
};
