// Fill out your copyright notice in the Description page of Project Settings.
#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorInitial_Z = GetOwner()->GetActorLocation().Z;
	DoorCurrent_Z = DoorInitial_Z;

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component, but no PressurePlate set!!!"), *GetOwner()->GetName());
	}
}
void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No AudioComponent is missing in %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > RequiredMassToOpenDoor)
	{
		LowerTheGate(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() >= DoorLastOpened + DoorCloseDelay) UpperTheGate(DeltaTime);
	}
}

void UOpenDoor::LowerTheGate(float DeltaTime)
{
	
	DoorCurrent_Z = FMath::FInterpConstantTo(DoorCurrent_Z, DoorTarget_Z, DeltaTime, OpeningSpeed);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Z = DoorCurrent_Z;
	GetOwner()->SetActorLocation(DoorLocation);

	if (!AudioComponent) return;

	if (bOpenSoundWasPlayed == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lower The Gate!"));
		AudioComponent->Play(); // open the gate
		bOpenSoundWasPlayed = true;
	}
}

void UOpenDoor::UpperTheGate(float DeltaTime)
{
	DoorCurrent_Z = FMath::FInterpConstantTo(DoorCurrent_Z, DoorInitial_Z, DeltaTime, OpeningSpeed);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Z = DoorCurrent_Z;
	GetOwner()->SetActorLocation(DoorLocation);
	
	if (!AudioComponent) return;
	
	if (bCloseSoundWasPlayed == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Upper The Gate!"));
		AudioComponent->Play(); // close the gate
		bCloseSoundWasPlayed = true;
		bOpenSoundWasPlayed = false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMassOfActor = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) // protect against nullptr
	{
		return TotalMassOfActor;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMassOfActor += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMassOfActor;
}
