// Fill out your copyright notice in the Description page of Project Settings.


#include "MyElevator.h"
#include "GameFramework/character.h"

// Sets default values
AMyElevator::AMyElevator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Root->SetupAttachment(RootComponent);
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(Root);
	LDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LDoor"));
	LDoor->SetupAttachment(Base);
	L_Coll = CreateDefaultSubobject<UBoxComponent>(TEXT("L_Coll"));
	L_Coll->SetupAttachment(LDoor);
	RDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RDoor"));
	RDoor->SetupAttachment(Base);
	R_Coll = CreateDefaultSubobject<UBoxComponent>(TEXT("R_Coll"));
	R_Coll->SetupAttachment(RDoor);

	Trig = CreateDefaultSubobject<UBoxComponent>(TEXT("Trig"));
	Trig->SetupAttachment(Root);
	Trig->SetVisibility(true);

	Base_Trig = CreateDefaultSubobject<UBoxComponent>(TEXT("Base_Trig"));
	Base_Trig->SetupAttachment(Base);
	Base_Trig->SetVisibility(true);

	TrigUp = CreateDefaultSubobject<UBoxComponent>(TEXT("TrigUp"));
	TrigUp->SetupAttachment(Base);
	TrigUp->SetVisibility(true);

	OpenCurve = CreateDefaultSubobject<UCurveFloat>("OpenCurve");
	CloseCurve = CreateDefaultSubobject<UCurveFloat>("CloseCurve");
	bIsOpen = false;
	bIsClose = false;
}

void AMyElevator::TimelineProgress(float Value)
{
	FRotator NewLocation = FMath::Lerp(StartLoc, EndLoc, Value);
	LDoor->SetRelativeRotation(-1 * NewLocation);
	RDoor->SetRelativeRotation(NewLocation);
}

// Called when the game starts or when spawned
void AMyElevator::BeginPlay()
{
	Super::BeginPlay();
	Trig->OnComponentBeginOverlap.AddDynamic(this, &AMyElevator::OnOverlapBegin);
	TrigUp->OnComponentBeginOverlap.AddDynamic(this, &AMyElevator::OnOverlapBegin);
}

void AMyElevator::OnOverlapBegin(UPrimitiveComponent* OnOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (bIsOpen) return;
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetClass()->GetName());
	if (LDoor->GetRelativeRotation() == FRotator(0.f, 0.f, 0.f))bIsOpen = false;
	if (Character && CurveFloat && OtherActor->GetClass()->GetName() == "BP_ThirdPersonCharacter_C" && !bIsOpen)
	{
		bIsOpen = true;
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		//CurveTimeline.SetLooping(true);
		CurveTimeline.Play();
		StartLoc = EndLoc = LDoor->GetRelativeRotation();
		EndLoc.Yaw += YOffset;
		CurveTimeline.SetNewTime(0.f);
		CurveTimeline.PlayFromStart();
	}

}

// Called every frame
void AMyElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
	
}

void AMyElevator::StartOpening()
{
	//CurveTime = 0.0f;
}

void AMyElevator::OnOverlapEnd(UPrimitiveComponent* OnOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called to bind functionality to input
void AMyElevator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

