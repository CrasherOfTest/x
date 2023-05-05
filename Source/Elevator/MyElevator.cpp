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
	RDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RDoor"));
	RDoor->SetupAttachment(Base);
	//Trigers for door
	Trig = CreateDefaultSubobject<UBoxComponent>(TEXT("Trig"));
	Trig->SetupAttachment(Root);
	Trig->SetVisibility(true);

	Base_Trig = CreateDefaultSubobject<UBoxComponent>(TEXT("Base_Trig"));
	Base_Trig->SetupAttachment(Base);
	Base_Trig->SetVisibility(true);
	//Floor StaticMeshes
	FirstFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstFloor"));
	FirstFloor->SetupAttachment(Base);
	/*TrigUp = CreateDefaultSubobject<UBoxComponent>(TEXT("TrigUp"));
	TrigUp->SetupAttachment(Base);
	TrigUp->SetVisibility(true);*/

	UPDownCurve = CreateDefaultSubobject<UCurveFloat>("OpenCurve");
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
void AMyElevator::TimelineProgress_B(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLoc_B, EndLoc_B, Value);
	Base->SetRelativeLocation(NewLocation);
}

// Called when the game starts or when spawned
void AMyElevator::BeginPlay()
{
	Super::BeginPlay();
	Trig->OnComponentBeginOverlap.AddDynamic(this, &AMyElevator::OnOverlapBegin);
	//TrigUp->OnComponentBeginOverlap.AddDynamic(this, &AMyElevator::OnOverlapBegin);
}

void AMyElevator::OnOverlapBegin(UPrimitiveComponent* OnOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (bIsOpen) return;
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetClass()->GetName());
	if (LDoor->GetRelativeRotation() == FRotator(0.f, 0.f, 0.f))bIsOpen = false;
	if (Base_Trig->GetRelativeLocation() == Trig->GetRelativeLocation() && Character && CurveFloat && OtherActor->GetClass()->GetName() == "BP_ThirdPersonCharacter_C" && !bIsOpen && OnOverlappedComponent->GetName() == "Trig")
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
	CurveTimeline_B.TickTimeline(DeltaTime);
}

void AMyElevator::FloorUp()
{

	FOnTimelineFloat TimelineProgress_B;
	TimelineProgress_B.BindUFunction(this, FName("TimelineProgress_B"));

	CurveTimeline_B.AddInterpFloat(UPDownCurve, TimelineProgress_B);
	//CurveTimeline.SetLooping(true);
	CurveTimeline_B.Play();
	StartLoc_B = EndLoc_B = Base->GetRelativeLocation();
	EndLoc_B.Z += YOffset;
	CurveTimeline_B.SetNewTime(0.f);
	CurveTimeline_B.PlayFromStart();
	UE_LOG(LogTemp, Warning, TEXT("asdfasd"));
}

void AMyElevator::OnOverlapEnd(UPrimitiveComponent* OnOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//End of overlaping
}



// Called to bind functionality to input
void AMyElevator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

